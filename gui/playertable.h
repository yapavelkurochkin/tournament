#ifndef PLAYERTABLE__H
#define PLAYERTABLE__H

#include <QListWidget>
#include "playerlist.h"

class PlayerTable: public QListWidget
{
  Q_OBJECT;

  public:
    PlayerTable( QWidget* parent = NULL );

    void setPlayerList( PlayerList pl ); 
    PlayerList playerList() const { return _players; }

  signals:
    void updated( PlayerList pl );

  protected:
    PlayerList _players;

    virtual void mouseDoubleClickEvent( QMouseEvent * event );
};

#endif // PLAYERTABLE__H
