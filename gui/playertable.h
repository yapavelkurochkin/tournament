#ifndef PLAYERTABLE__H
#define PLAYERTABLE__H

#include <QTableWidget>
#include "playerlist.h"

class PlayerTable: public QTableWidget 
{
  public:
    PlayerTable( PlayerList pl, QWidget* parent = NULL );

    void setPlayerList( PlayerList pl );
};

#endif // PLAYERTABLE__H
