#include <QDebug>
#include <math.h>
#include "rrgroup.h"

RRGroup::RRGroup( QChar name, PlayerList players )
 : _name( name ),
   _players( players )
{
  roundRobin();
}

void RRGroup::roundRobin()
{
  _matches.clear();

  for ( int i = 0; i < _players.count(); i ++ ) {
    for ( int j = i + 1; j < _players.count(); j ++ ) {
      qDebug() << __FUNCTION__ << _players.at( i ).name() << _players.at( j ).name(); 
      _matches << Match( _players.at( i ), _players.at( j ) );
    }
  }
}

void RRGroup::addPlayer( Player player )
{
  _players << player;
  roundRobin();
}

/** Find match which corresponds to player 'a' and 'b' pair.
 */
Match& RRGroup::match( Player a, Player b )
{
  qDebug() << __FUNCTION__ << "searching for a match of" << a.name() << "and" << b.name();
  qDebug() << __FUNCTION__ << "matches count" << _matches.count();
  for ( int i = 0; i < _matches.count(); i ++ ) { 
    Match m = _matches.at( i );
    qDebug() << __FUNCTION__ << m.playerA().name() << m.playerB().name();
    if ( ( ( m.playerA() == a ) && ( m.playerB() == b ) ) ||
         ( ( m.playerB() == a ) && ( m.playerA() == b ) ) ) {
      qDebug() << __FUNCTION__ << "Found match!";
      return _matches[ i ];
    } 
  }

  // should never get here!  
  return (* new Match( a, b ) );
}

/** Sorts player list and pushes cool players into first places
 * of groups.
 * Function assumes that 'players' is sorted list.
 */
RRGroupList breakPlayers( PlayerList players, int groupSize )
{
  int groupCnt = ceil( (double) players.count() / groupSize );
  
  RRGroupList groups;

  for ( int i = 0; i < groupCnt; i ++ ) {
    groups << RRGroup( QChar( 'A' + i ) );
  }

  while ( players.count() ) {
    for ( int i = 0; ( i < groupCnt ) && players.count(); i ++ ) {
      Player player = players.takeLast();
     
      groups[i].addPlayer( player );
    }
  }
  
  return groups;
}

