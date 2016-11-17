#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "playertable.h"

PlayerTable::PlayerTable( QWidget* parent )
: QListWidget( parent )
{
  setViewMode( QListView::ListMode );

  setSelectionMode( QAbstractItemView::NoSelection );
  setSelectionRectVisible( false );

  setFrameStyle( QFrame::Panel | QFrame::Sunken );
}

void PlayerTable::setPlayerList( PlayerList pl )
{
  _players = pl;
  qSort( _players );

  clear();

  for ( int i = 0; i < _players.count(); i ++ ) {
    QString text = QString::number( _players.at( i ).rating(), 'f', 1 ) + 
                   + " " + _players.at( i ).name();

    addItem( text );
  }
}

/**
 * Asks user to select a file with players in a plain text format:
 * Player, rating\nPlayer2, rating\n etc...
 */
void PlayerTable::mouseDoubleClickEvent( QMouseEvent * )
{
  QString fName = QFileDialog::getOpenFileName(this,
                  tr("Open players list file"), "", 
                  tr("Txt Files (*.txt)"));

  if ( !fName.isNull() ) {
    PlayerList players = loadPlayerList( fName );
    if ( players.count() > 0 ) {
      setPlayerList( players );
      emit updated( players );
    } else {
      QMessageBox msg;
      msg.setText( tr( "I cannot find any player in '" ) + fName + "'" );
      msg.exec();
    } 
  }
}
