#include <math.h>
#include <QDebug>
#include <QFile>
#include <QDir>

#include "tournament.h"
#include "rrgroup.h"
#include "swissgroup.h"


unsigned int log2( unsigned int x )
{
  unsigned int ans = 0 ;
  while( x>>=1 ) ans++;
  return ans ;
}

Tournament::Tournament( PlayerList players, QString category,
                        Match::Type matchType, unsigned int groupCnt )
                        
 : _magic( TOURN_MAGIC_NUMBER ),
   _groupCnt( groupCnt ),
   _stagesCnt( 0 ),
   _rrBreakAlgo( ADBC ),
   _matchType( matchType ),
   _category( category )
{
  // emperical formula, I can't describe it..
	// groupCnt = 2 -> stagesCnt = 4
	// groupCnt = 4 -> stagesCnt = 5
	// groupCnt = 8 -> stagesCnt = 6
  _stagesCnt = log2( groupCount() * 4 ) + 1;

  _groups = new QList<Group*>[ _stagesCnt ];
  Q_CHECK_PTR( _groups );

  breakPlayers( players );
  
  emit tournamentChanged( this );
}

/** This constructor is only intended for serialization/deserialization
 */
Tournament::Tournament( ) 
 : _magic( 0xdeadbeef ), // object is not invalid
   _groupCnt( 0 ),
   _stagesCnt( 0 ),
   _matchType( Match::BestOf3 ),
   _category( "M2" ) 
{
}

unsigned int Tournament::groupCount() const 
{
	return _groupCnt; 
}

void Tournament::groupChanged( Group* g )
{
  if ( g->stage() == 0 ) {
    if ( roundRobinCompleted() ) {
      buildGroups( ); 
    }
  } else {
    if ( g->completed() && ( g->stage() != ( _stagesCnt - 1 ) ) ) {
      splitSwissGroup( dynamic_cast< SwissGroup* >( g ) ); 
    }
  }

  emit tournamentChanged( this );
}

/** build groups by results of round-robin stage 
 *   \todo this code should be in split() method of RRGroup
 */
void Tournament::buildGroups( )
{
  // 1st stage group size.
  // groupCnt = 2 => gs(1) = 4
  // groupCnt = 4 => gs(1) = 8
  // groupCnt = 8 => gs(1) = 16
	//
	// first gs players are playing 1-2 2-1 1-2 2-1
  int gs = groupCount() * 2; 
  PlayerList players = roundRobinResults( );

  qDebug() << __PRETTY_FUNCTION__ << players.count();

  newSwissGroup( 1, 1, players.mid( 0, gs ) );

  players = players.mid( gs ); 
	// next gs/2 players are playing 3-3 4-4 5-5, etc..
  int fromPlace = gs;
  while ( ( gs = ( gs / 2 ) ) >= 2 ) {
	  while ( gs <= players.count() ) {
      newSwissGroup( fromPlace + 1, 1, players.mid( 0, gs ) );
			players = players.mid( gs );
			fromPlace += gs;
		}
  } 
}

/** splits groups for two groups - group of winners and group of loosers.
 */
void Tournament::splitSwissGroup( SwissGroup* g )
{
  QList< Group* > groups = g->split();   

  // if group contains only 1 match then it will not be splitted
  // and 

  if ( groups.count() ) {
    _groups[ groups.at( 0 )->stage() ] << groups;
  }

  for ( int i = 0; i < groups.count(); i ++ ) {
    emit newSwissGroupCreated( dynamic_cast< SwissGroup* >( groups.at( i ) ) );
  }
}

SwissGroup* Tournament::newSwissGroup( unsigned int fromPlace, unsigned int stage, 
                                       PlayerList players )
{
  SwissGroup* sg = new SwissGroup( fromPlace, this, stage, players );

  _groups[ 1 ] << sg;
  emit newSwissGroupCreated( sg );
  return sg;
}

/** Function assumes that 'players' is sorted list.
 *  Breaking is done in 'snake' manner (up->down, down->up, etc...):
 *   1 8 9  16
 *   2 7 10 15
 *   3 6 11 14
 *   4 5 12 13
 */
void Tournament::breakPlayers( PlayerList players )
{
  int groupCnt = groupCount(); 
  
  _groups[0].clear(); 

  for ( int i = 0; i < groupCnt; i ++ ) {
    _groups[0] << new RRGroup( QChar( 'A' + i ), this );
  }

  int snake = 1;
  int dir = 1; // +1 - up direction, -1 - down direction 

  while ( players.count() ) {
    int i_start = 0, i_end = 0;

    if ( dir < 0 ) { // up -> down
      i_start = groupCnt - 1;
      i_end = -1;
    } else { // down -> up
      i_start = 0;
      i_end = groupCnt;
    }

    int i = i_start;

    do {
      Player player = players.takeLast();
      _groups[0][i]->addPlayer( player );
      
      qDebug() << player.name() << player.rating() << i;
    
      i += dir;

    } while ( ( i != i_end ) && players.count() ); 
    
    if ( snake ) { // change iteration direction 
      dir = ( dir < 0 ) ? 1 : -1;
    }
  }
}

bool Tournament::roundRobinCompleted( ) const
{
  for ( int i = 0; i < _groups[ 0 ].count(); i ++ ) {
    if ( ! _groups[ 0 ].at( i )->completed() ) {
      return false;
    }
  }
 
  return true;
}

/** \return player list built by magic principles. 
 *          They depend from the breaking algorithm selected by admin (ABCD, ADBC, ACBD) 
 *          Firsts plays with seconds, Seconds with firsts.
 *          Third plays with third, and so on.
 *
 *          Resulting list should be used simply: first item plays with second, third 
 *          with fourth and so on.
 */
PlayerList Tournament::roundRobinResults() const
{
  PlayerList list;

  int maxGroupSize = 0;
  unsigned cnt = _groups[0].count();
  for ( unsigned i = 0; i < cnt; i ++ ) {
    const Group* g = _groups[0].at( i );
    if ( g->size() > maxGroupSize ) {
      maxGroupSize = g->size(); 
    }
  }

  // we know that groups count is always even. 
  
  QList<unsigned> indexes;

  if ( rrBreakAlgo() == Tournament::ABCD ) {
    for ( unsigned i = 0; i < cnt; i++ ) {
      indexes << i;
    }
  }

  if ( rrBreakAlgo() == Tournament::ADBC ) {
    // ABCD -> ADBC
    // ABCDEFGH -> AHBGCFDE
    for ( unsigned i = 0; i < cnt / 2; i++ ) {
      indexes << i;
      indexes << cnt - 1 - i;
    }
  }

  if ( rrBreakAlgo() == Tournament::ACBD ) {
    // ABCD -> ACBD
    // ABCDEFGH -> AEBFCGDH
    for ( unsigned i = 0; i < cnt / 2; i++ ) {
      indexes << i;
      indexes << cnt/2 + i;
    }
  }

  for ( int p = 1; p <= maxGroupSize; p ++ ) {
    for ( unsigned i = 0; i < cnt; i ++ ) {
      const Group* g = _groups[0].at( indexes.at( i ) );

      int place = p;
      if ( i & 0x1 ) { // odd groups
        if ( p == 1 ) // swapping first and second players in odd groups
          place = 2;
        if ( p == 2 )
          place = 1;
        // third and fourth players stays unswapped
      } 

      if ( place <= g->size() ) {
        list << g->playerByPlace( place );
      }
    }
  }

  return list;
}

/** build actual player list. note, tha playerlist can be changed
 *  by user (rrtable allows to add or remove user via menu).
 *  that is why Tournament class does not store player list but builds it 
 *  on demand.
 */
PlayerList Tournament::players() const
{
  PlayerList list;
  for ( int i = 0; i < _groups[0].count(); i ++ ) {
    const Group* g = _groups[0].at( i );
    list << g->const_players();
	}
   
  return list;
}

bool Tournament::save( QString fname )
{
  QFile file( fname );
  return save( &file );
}

bool Tournament::save( QFile* f )
{
  if ( f->open( QIODevice::WriteOnly ) ) {
    QDataStream stream( f );
    stream << (*this);
    setFileName( f->fileName() );
    f->close();
    return true;
  } 

  return false;
}

/** creates an instance of Tournament initialized from specified file.
 */
Tournament* Tournament::fromFile( QString fileName )
{
  QFile file( fileName );
  Tournament* t = NULL;
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
    t = new Tournament();

    QDataStream ds( &file );
    
    ds >> (*t);
    t->_fileName = fileName; 
  } 

  return t;
}

/** \return total matches list
 */
MatchList Tournament::matchList() const 
{
  MatchList ml;
  for ( unsigned int i = 0; i < stagesCnt(); i ++ ) {
    QList<Group*> gl = groupList( i );
    for ( int j = 0; j < gl.count(); j ++ ) {
      ml << gl.at( j )->matchList();
    }   
  }

  return ml;
}

/* serialization
 */
QDataStream &operator>>(QDataStream &s, Tournament& t)
{
  int mType;
  QString cat;

  if ( s.atEnd() ) {
	  return s;
	}
 
  s >> t._magic; 

  if ( t._magic != TOURN_MAGIC_NUMBER ) {
    // invalid data source. object is not valid tournament
    qWarning() << __FUNCTION__ << "invalid datastream "
    "(can't initialize Tournament object from it's contents)";
    return s;
  }
  
  int rrBreakAlgo = Tournament::ADBC;
  s >> t._groupCnt >> t._stagesCnt >> mType >> cat >> rrBreakAlgo; 

  t._rrBreakAlgo = (Tournament::RRBreakAlgorithm) rrBreakAlgo;
  t._category = cat;
  t._matchType = (Match::Type) mType;
  t._groups = new QList<Group*>[ t._stagesCnt ];

  for ( unsigned int i = 0; i < t._stagesCnt; i ++ ) {
    int count;
    s >> count;
    for ( int j = 0; j < count; j ++ ) {
      if ( i == 0 ) { // round robin stage
        RRGroup* rrg = new RRGroup();
        s >> (*rrg);
 
        rrg->setTournament( &t );
        t._groups[i] << rrg; 
      } else {
        SwissGroup* sg = new SwissGroup();
        s >> (*sg);
        
        sg->setTournament( &t );
        t._groups[i] << sg; 
      }
    }   
  }
 
  return s;
}

QDataStream &operator<<(QDataStream &s, const Tournament& t)
{
  if ( !t.isValid() ) {
    qWarning() << __FUNCTION__ << 
                  "trying to serialize invalid tournament object";
    return s;
  }
  
  s << t._magic << t._groupCnt << t._stagesCnt 
    << (int) t._matchType << t._category << (int)t._rrBreakAlgo; 

  for ( unsigned int i = 0; i < t._stagesCnt; i ++ ) {
    int count = t._groups[ i ].count();
    s << count;
    for ( int j = 0; j < count; j ++ ) {
      const Group* g = t._groups[ i ].at( j );
      if ( i == 0 ) { // round robin stage
        const RRGroup* rrg = dynamic_cast< const RRGroup* >( g );

        s << (*rrg);
      } else {
        const SwissGroup* sg = dynamic_cast< const SwissGroup* >( g );
         
        s << (*sg);
      }
    }   
  }

  return s;
}

/** prints the results to csv file,
 * '|' is the separator
 */
void Tournament::saveAsCSV( QString file )
{
  if ( !isValid() ) {
    qWarning() << __FUNCTION__ << 
                  "trying to save a results of invalid tournament object";
    return;
	}
 

	QFile csv( file );
	QChar sep( '|' );

	if (csv.open(QFile::WriteOnly | QFile::Truncate)) {
		QTextStream out(&csv);
  
	  for ( unsigned int i = 0; i < _stagesCnt; i ++ ) {
      int count = _groups[ i ].count();
      for ( int j = 0; j < count; j ++ ) {
        const Group* g = _groups[ i ].at( j );
				out << g->csvResult( sep ) << endl;
      }
			out << endl;
    }

		out << totalRatingAsCSV( sep ) << endl;
  }
}

/** prints total ratings as CSV. there are 4 columns:
 *   player name, last rating, earned rating, total rating.
 *   list is sorted by last rating (from higher to lower)
 */
QString Tournament::totalRatingAsCSV( QChar sep )
{
  QString ret;
	QTextStream out( &ret );

  PlayerList pls = players();
	qSort( pls.end(), pls.begin() );

  Group fake( "unused", this, matchList(), pls );
  for ( int i = 0; i < pls.count(); i ++ ) {
	  Player p = pls.at( i );
		double earned = fake.earnedRating( p ); 
		double total = p.rating() + earned;
		out << p.name() << sep << p.ratingAsStr() << sep
		    << "+" + QString::number( earned, 'f', 1 ) << sep
		    << QString::number( total , 'f', 1 ) << endl;
	}

	return ret;
}
