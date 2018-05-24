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

  for ( unsigned int i = 0; i < props().rrGroupNum; i ++ ) {
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

    foreach( Group *g, groups ) {
      dynamic_cast< SwissGroup* >(g)->permuteMatches( breakAlgo() );
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
  unsigned max = maxGroupSize( groups );

  PlayerList winners;
  for ( unsigned i = 0; i < cnt; i ++ ) {
    const Group* g = groups.at( i );

    for ( unsigned p = 1; p <= 2; p ++ ) {
      winners << g->playerByPlace( p );
    }
  }

  // should be at least 2 groups, 2 winners in each
  Q_ASSERT( winners.count() >= 4 );

  for ( int i = 0; i < winners.count(); i += 4 ) {
    list << winners.at( i );
    list << winners.at( i + 3 );
    list << winners.at( i + 1 );
    list << winners.at( i + 2 );
  }

  for ( unsigned p = 3; p <= max; p ++ ) {
    for ( unsigned i = 0; i < cnt; i ++ ) {
      const Group* group = groups.at( i );
      if ( p <= (unsigned) group->size() ) {
	list << group->playerByPlace( p );
      }
    }
  }

  return list;

}

