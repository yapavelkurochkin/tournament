#include <QList>
#include "tourndata.h"
#include "tournalgo.h"
#include "group.h"
#include "tournament.h"
#include "swissgroup.h"

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
  for ( int i = 0; i < _groups[0].count(); i ++ ) {
    const Group* g = _groups[0].at( i );
    list << g->const_players();
	}
   
  return list;
}

/** \return total matches list
 */
MatchList TournData::matchList( int stage ) const 
{
  MatchList ml;
  if ( stage >= 0 ) {
    for ( int j = 0; ( stage < _algo->stagesCnt() ) && ( j < _groups[ stage ].count() ); j ++ ) {
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
 */
 void TournData::groupChanged( Group *g )
{
  Q_ASSERT( g != NULL );
  Q_ASSERT( _tournament != NULL );

  if ( g->stage() == 0 ) {
    // 0th stage is always qualification (RoundRobin or another one).
    // it should be fully completed before next stage.
    if ( _algo->stageCompleted( _groups[0] ) ) {
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

