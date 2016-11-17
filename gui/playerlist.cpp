#include <QFile>
#include <QDebug>
#include <QStringList>
#include "playerlist.h"

/** File should have comma-separated format, like this:
 *    Player2 name, player1 rating
 *    Player3 name, player3 rating
 *
 *  \sa Player
 *  \return unsorted players list or empty list if file not found 
 *          or wrong format
 */
PlayerList loadPlayerList( QString fname )
{
  QFile f ( fname );
  PlayerList ret;
  if ( f.open( QIODevice::ReadOnly ) ) {
    QString contents = QString::fromUtf8( f.readAll() );
    QStringList list = contents.split( '\n' );

    qDebug() << contents;
    for ( int i = 0; i < list.count(); i ++ ) {
      QStringList plVals = list.at(i).split( ',' );

      if ( plVals.count() >= 2 ) {
        Player player( plVals.at( 0 ), plVals.at( 1 ).toDouble() );
 
        ret << player;
      }
    }
  }

  return ret;
}

// at least one real player => false
// all players = bye  => true
bool isPlayerListByed( PlayerList pl )
{ 
  foreach( Player p, pl ) {
    if ( !p.isBye() ) {
      return false;
    }
  }

  return true;
} 
