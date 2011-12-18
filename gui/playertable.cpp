#include "playertable.h"

PlayerTable::PlayerTable( PlayerList pl, QWidget* parent )
: QTableWidget( pl.count(), 2, parent )
{
  setPlayerList( pl );
}

void PlayerTable::setPlayerList( PlayerList pl )
{
  for ( int i = 0; i < rowCount(); i ++ ) {
    QTableWidgetItem *name = new QTableWidgetItem( pl.at( i ).name() );
    QTableWidgetItem *rating = new QTableWidgetItem( 
                        QString::number( pl.at( i ).rating(), 'f', 1 ) );

    setItem( i, 0, name );
    setItem( i, 1, rating );
  }
}
