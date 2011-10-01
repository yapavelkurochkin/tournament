#include <QDebug>
#include <math.h>
#include "rrgroup.h"

RRGroup::RRGroup( QChar name, PlayerList players )
 : _name( name ),
   _players( players )
{
  for ( int i = 0; i < _players.count(); i ++ ) {
    for ( int j = i + 1; j < _players.count(); j ++ ) {
      _matches << Match( _players.at( i ), _players.at( j ) );
    }
  }
}

/** Find match which corresponds to player 'a' and 'b' pair.
 */
Match& RRGroup::match( Player a, Player b )
{
  for ( int i = 0; i < _matches.count(); i ++ ) { 
    Match m = _matches.at( i );
    if ( ( ( m.playerA() == a ) && ( m.playerB() == b ) ) ||
         ( ( m.playerB() == a ) && ( m.playerA() == b ) ) ) {
      return _matches[ i ];
    } 
  }
 
  return (* new Match( a, b ) );
}

/** Sorts player list and pushes cool players into first places
 * of groups.
 * Function assumes that 'players' is sorted list.
 */
RRGroupList breakPlayers( PlayerList players, int groupSize )
{
  int groupCnt = ceil( (double) players.count() / groupSize );
  
  qDebug() << __FUNCTION__ << groupCnt;

  RRGroupList groups;

  for ( int i = 0; i < groupCnt; i ++ ) {
    groups << RRGroup( QChar( 'A' + i ) );
  }

  while ( players.count() ) {
    for ( int i = 0; ( i < groupCnt ) && players.count(); i ++ ) {
      Player player = players.takeLast();
     
      groups[i].players() << player;
    }
  }

  return groups;
}

