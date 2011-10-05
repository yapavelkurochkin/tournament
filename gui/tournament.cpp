#include <math.h>

#include "tournament.h"
#include "rrgroup.h"

Tournament::Tournament( PlayerList players, Category category,
                        Match::Type matchType, int groupSize)
 : _players( players ),
   _groupSize( groupSize ),
   _matchType( matchType ),
   _category( category ) 
{
   breakPlayers( _players );
}

void Tournament::groupChanged( Group* g )
{
}

/** Sorts player list and pushes cool players into first places
 * of groups. Less cool players into second places and so on.
 * Function assumes that 'players' is sorted list.
 */
void Tournament::breakPlayers( PlayerList players )
{
  int groupCnt = ceil( (double) players.count() / _groupSize );
  
  _groupList.clear(); 

  for ( int i = 0; i < groupCnt; i ++ ) {
    _groupList << RRGroup( QChar( 'A' + i ), this );
  }

  while ( players.count() ) {
    for ( int i = 0; ( i < groupCnt ) && players.count(); i ++ ) {
      Player player = players.takeLast();
     
      _groupList[i].addPlayer( player );
    }
  }
}


