#ifndef PLAYERLIST__H
#define PLAYERLIST__H

#include <QList>
#include "player.h"

typedef QList<Player> PlayerList;

extern PlayerList loadPlayerList( QString file );
bool isPlayerListByed( PlayerList ); 
PlayerList rndPermPlayerList( PlayerList );

#endif // PLAYERLIST__H

