#include <QDebug>
#include <QList>
#include "tourndata.h"
#include "tournalgo.h"
#include "group.h"
#include "tournament.h"
#include "swissgroup.h"
#include "lbrdialog.h"

TournData::TournData( const TournAlgo *algo  )
:_algo( algo ),
 _tournament( NULL )
{
}

void TournData::initGroups( )
{
  Q_CHECK_PTR( _algo != NULL );
  
  if ( _algo->stagesCnt() > 0 ) {
    _groups = new QList<Group*>[ _algo->stagesCnt() ];
    Q_CHECK_PTR( _groups );

    _groups[ 0 ] = _algo->buildGroups( 0 );
    
    foreach( Group *g, _groups[ 0 ] ) {
      g->setTournData( this ); 
    }
  }
}

/** build actual player list. NOTE, that playerlist can be changed
 *  by user (rrtable allows to add or remove user via menu).
 *  that is why Tournament class does not store player list but builds it 
 *  on demand.
 *
 * \todo this function should be used for synchonization between actual list
 *       of players and initial list passed as TournProps
 */
PlayerList TournData::playerList() const
{
  PlayerList list;
  for ( int i = 0; i < (int)_algo->stagesCnt(); i ++ ) {
    QList<Group*> gl = _groups[ i ];
    for ( int j = 0; j < gl.count(); j ++ ) {
      const Group* g = gl.at( j );
      list << g->const_players();
    }
  }

  // removing duplicates by converting to hash-based set and back.
  QSet<Player> set = list.toSet();
  return set.toList();
}

/** \return total matches list
 */
MatchList TournData::matchList( int stage ) const 
{
  MatchList ml;
  if ( stage >= 0 ) {
    for ( int j = 0; ( stage < (int)_algo->stagesCnt() ) && ( j < _groups[ stage ].count() ); j ++ ) {
      ml << _groups[ stage ].at( j )->matchList();
    }   
  } else { 
    for ( unsigned int i = 0; i < _algo->stagesCnt(); i ++ ) {
      QList<Group*> gl = _groups[ i ];
      for ( int j = 0; j < gl.count(); j ++ ) {
	ml << gl.at( j )->matchList();
      }   
    }
  }
  return ml;
}

/** Called each time when match of group is completed.
 *  Function builds groups when some stage finished or full swissgroup
 *  completed.
 *
 *  \todo it seems that some code should be moved into buildGroups() function
 */
 void TournData::groupChanged( Group *g )
{
  Q_ASSERT( g != NULL );
  Q_ASSERT( _tournament != NULL );

  if ( ( g->stage() == 0 ) 
       && ( ( algo()->props_const().type == TournProps::RRPlayOff ) 
            || ( algo()->props_const().type == TournProps::QualifPlayOff ) ) ) {
    // 0th stage is for qualification (RoundRobin or another one).
    // it should be fully completed before next stage.
    if ( _algo->stageCompleted( _groups[0] ) ) {

      // we give manager a possibility to select how to break loser bracket: to
      // play shortly (3-3, 4-4, ...) or to play full playoff (like winners do
      // after round-robin stage)
      if ( algo()->props_const().type == TournProps::RRPlayOff ) {
        LBrDialog d(NULL);
        d.exec();
        // dirty hack here ;(
        ((TournAlgo*)_algo)->props()->lBrBreakType = d.lBrBreakType();
      }

      _groups[1] = _algo->buildGroups( 1, _groups[0] );
      foreach( Group *gr, _groups[ 1 ] ) {
        gr->setTournData( this );
      }
    }
  } else {
    if ( g->completed() && ( !_algo->isStageLast( g->stage() ) ) ) {
      _groups[ g->stage() + 1 ] << dynamic_cast< SwissGroup* >( g )->split();
      // split() function should set tourn data 
    }
  }

  if ( _tournament ) {
     _tournament->update();
  }
}
 
static bool resLessThan( const QPair< unsigned int, Player > &p1,
                         const QPair< unsigned int, Player > &p2 )
{
  return p1.first < p2.first;
}
                           

/** \return sorted list of players accordingly to their places in tournament.
 */
QList< QPair< unsigned int, Player > > TournData::results() const
{
  QList< QPair< unsigned int, Player > > list;

  // 0th stage can't define a place
  for ( unsigned int s = 1; s < _algo->stagesCnt(); s++ ) {
    foreach( Group * g, groupList( s ) ) { 
      SwissGroup *sg = dynamic_cast< SwissGroup* >( g );
      if ( sg ) {
        list << sg->absPlaces( );
      }
    } 
  }

  qSort( list.begin(), list.end(), resLessThan );

  return list; 
}

    

// t.algo should be valid before loading from datastream
QDataStream &operator>>(QDataStream &s, TournData& t )
{
  if ( !t.algo() ) {
    return s;
  }

	t._groups = new QList<Group*>[ t.algo()->stagesCnt() ];
  
	for ( unsigned int i = 0; i < t.algo()->stagesCnt(); i ++ ) {
		int count;
		s >> count;

		for ( int j = 0; j < count; j ++ ) {
			QString type;
			s >> type;
			
			if ( type == "round-robin" ) {
				RRGroup* rrg = new RRGroup();
				s >> (*rrg);
				rrg->setTournData( &t );
  			t._groups[i] << rrg; 
			} else if ( type == "swiss" ) {
				SwissGroup* sg = new SwissGroup();
				s >> (*sg);

				sg->setTournData( &t );
				t._groups[i] << sg; 
			} else {
				qWarning() << "unknown group type. Stop loading from datastream";
				return s;
			}
		}   
	}

  return s;
}

QDataStream &operator<<(QDataStream &s, const TournData& t )
{
  if ( !t.algo() ) {
    return s;
  }

	for ( unsigned int i = 0; i < t.algo()->stagesCnt(); i ++ ) {
    int count = t._groups[i].count();

    s << count;    

		for ( int j = 0; j < count; j ++ ) {
      Group *g = t._groups[i].at( j );
			QString type = g->type();
      
			s << type;
			
			if ( type == "round-robin" ) {
				s << (*dynamic_cast< RRGroup* >( g ));
			} else if ( type == "swiss" ) {
				s << (*dynamic_cast< SwissGroup* >( g ));
			} else {
				qCritical() << "unknown group type:" << type;
				return s;
			}
		}   
	}
  
  return s;
}
