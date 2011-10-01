#ifndef PLAYERLIST__H
#define PLAYERLIST__H

#include <QList>
#include "player.h"

typedef QList<Player> PlayerList;

extern PlayerList loadPlayerList( QString file );

#endif // PLAYERLIST__H

