#include <QDebug>
#include "rrplayoff.h"
#include "rrgroup.h"
#include "swissgroup.h"
#include "tournament.h"

/** calculate integer log2. log(9) = 3, log2( 16) = 4. 
 */
unsigned int log2( unsigned int x )
{
  unsigned int ans = 0 ;
  while( x>>=1 ) ans++;
  return ans ;
}

RRPlayoffAlgo::RRPlayoffAlgo( TournProps p )
: TournAlgo( p )
{
  // emperical formula, I can't describe it..
	// groupCnt = 2 -> stagesCnt = 4
	// groupCnt = 4 -> stagesCnt = 5
	// groupCnt = 8 -> stagesCnt = 6
  _stagesCnt = log2( p.rrGroupNum * 4 ) + 1;
}

/**
 *  List of players is sorted at first.
 *  Than, breaking is done in 'snake' manner (up->down, down->up, etc...):
 *   1 8 9  16
 *   2 7 10 15
 *   3 6 11 14
 *   4 5 12 13
 */
QList<Group*> RRPlayoffAlgo::initGroups( ) const
{
  QList< Group* > groups;
  PlayerList players = props().players;
  qSort( players );

  for ( int i = 0; i < props().rrGroupNum; i ++ ) {
    groups << new RRGroup( QChar( 'A' + i ) );
  }

  int snake = 1;
  int dir = 1; // +1 - up direction, -1 - down direction 

  while ( players.count() ) {
    int i_start = 0, i_end = 0;

    if ( dir < 0 ) { // up -> down
      i_start = props().rrGroupNum - 1;
      i_end = -1;
    } else { // down -> up
      i_start = 0;
      i_end = props().rrGroupNum;
    }

    int i = i_start;

    do {
      Player player = players.takeLast();
      groups[i]->addPlayer( player );
      
      qDebug() << player.name() << player.rating() << i;
    
      i += dir;

    } while ( ( i != i_end ) && players.count() ); 
    
    if ( snake ) { // change iteration direction 
      dir = ( dir < 0 ) ? 1 : -1;
    }
  }

  return groups;
}

/** build groups by results of round-robin stage 
 *   \todo this code should be in split() method of RRGroup
 */
QList<Group*> RRPlayoffAlgo::buildGroups( unsigned int stage, 
                                          QList<Group*> prevGroups ) const
{
  if ( stage >= stagesCnt() ) {
    return QList< Group* >();
  }
  if ( stage == 0 ) {
    return initGroups( );
  }

  if ( stage == 1 ) { // next to Round-Robin stage
    QList< Group* > groups;

    // 1st stage group size.
    // groupCnt = 2 => gs(1) = 4
    // groupCnt = 4 => gs(1) = 8
    // groupCnt = 8 => gs(1) = 16
  	//
	  // first gs players are playing 1-2 2-1 1-2 2-1   
		int gs = props().rrGroupNum * 2; 
		PlayerList players = roundRobinResults( prevGroups );

		qDebug() << __PRETTY_FUNCTION__ << "players count:", players.count();

		groups << new SwissGroup( 1, stage, players.mid( 0, gs ) );

		players = players.mid( gs ); 
		// next gs/2 players are playing 3-3 4-4 5-5, etc..
		int fromPlace = gs;
		while ( ( gs = ( gs / 2 ) ) >= 2 ) {
			while ( gs <= players.count() ) {
				groups << new SwissGroup( fromPlace + 1, stage, players.mid( 0, gs ) );
				players = players.mid( gs );
				fromPlace += gs;
			}
		}

    return groups;
  }

  return QList< Group* >();
}

/** \return player list built by magic principles. 
 *          They depend from the breaking algorithm selected by admin (ABCD, ADBC, ACBD) 
 *          Firsts plays with seconds, Seconds with firsts.
 *          Third plays with third, and so on.
 *
 *          Resulting list should be used simply: first item plays with second, third 
 *          with fourth and so on.
 */
PlayerList RRPlayoffAlgo::roundRobinResults( QList< Group* > groups ) const
{
  PlayerList list;

  unsigned cnt = groups.count();

  // we assume that groups count is even!
  
  QList<unsigned> indexes;

  if ( breakAlgo() == BreakAlgo::ABCD ) {
    for ( unsigned i = 0; i < cnt; i++ ) {
      indexes << i;
    }
  }

  if ( breakAlgo() == BreakAlgo::ADBC ) {
    // ABCD -> ADBC
    // ABCDEFGH -> AHBGCFDE
    for ( unsigned i = 0; i < cnt / 2; i++ ) {
      indexes << i;
      indexes << cnt - 1 - i;
    }
  }

  if ( breakAlgo() == BreakAlgo::ACBD ) {
    // ABCD -> ACBD
    // ABCDEFGH -> AEBFCGDH
    for ( unsigned i = 0; i < cnt / 2; i++ ) {
      indexes << i;
      indexes << cnt/2 + i;
    }
  }

  unsigned max = maxGroupSize( groups );
  if ( cnt == 8 ) {
    #define GROUP( symb ) groups.at( symb - 'A' )
    // we use special algorithm by Ehab Aljamal ;)
    // first sixteen players are built into pairs so:
    // A1-H2, E2-G1, B2-E1, C2-D1, C1-D2, F1-A2, F2-H1, G2-B1
    // other players plays 'place to place' accordingly to 
    // breakAlgo() selected
    list << GROUP( 'A' )->playerByPlace( 1 );
    list << GROUP( 'H' )->playerByPlace( 2 );
    list << GROUP( 'E' )->playerByPlace( 2 );
    list << GROUP( 'G' )->playerByPlace( 1 );
    list << GROUP( 'B' )->playerByPlace( 2 );
    list << GROUP( 'E' )->playerByPlace( 1 );
    list << GROUP( 'C' )->playerByPlace( 2 );
    list << GROUP( 'D' )->playerByPlace( 1 );
    list << GROUP( 'C' )->playerByPlace( 1 );
    list << GROUP( 'D' )->playerByPlace( 2 );
    list << GROUP( 'F' )->playerByPlace( 1 );
    list << GROUP( 'A' )->playerByPlace( 2 );
    list << GROUP( 'F' )->playerByPlace( 2 );
    list << GROUP( 'H' )->playerByPlace( 1 );
    list << GROUP( 'G' )->playerByPlace( 2 );
    list << GROUP( 'B' )->playerByPlace( 1 );

    for ( unsigned p = 3; p <= max; p ++ ) {
      for ( unsigned i = 0; i < cnt; i ++ ) {
        const Group* group = groups.at( indexes.at( i ) );
        if ( p <= (unsigned) group->size() ) {
          list << group->playerByPlace( p );
        }
      }
    }
  } else {
    for ( unsigned p = 1; p <= max; p ++ ) {
      for ( unsigned i = 0; i < cnt; i ++ ) {
        const Group* g = groups.at( indexes.at( i ) );
  
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
  }

  return list;

}

