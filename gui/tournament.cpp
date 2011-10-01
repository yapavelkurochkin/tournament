#include "tournament.h"

Tournament::Tournament( PlayerList players, Category category,
                        Match::Type matchType, int groupSize)
 : _players( players ),
   _groupSize( groupSize ),
   _groupList( breakPlayers( players, groupSize ) ),
   _matchType( matchType ),
   _category( category ) 
{

}
