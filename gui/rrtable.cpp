#include <QHeaderView>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

#include "playertable.h"
#include "rrtable.h"
#include "matchres.h"
#include "global.h"

/** Round-robin table. Represents one round-robin group of players.
 */
RRTable::RRTable( Group* group, QWidget* parent )
: GroupTable( group, parent ) 
{
  setupCells();

  connect( this, SIGNAL( cellDoubleClicked( int, int ) ),
           this, SLOT( editMatchResults( int, int ) ) );

  updateMatchCells();
  updatePlaces();

  resizeColumnsToContents();
  adjustSize();
}

/** should be called in constructor for basic setup of 
 * table cells.
 */
void RRTable::setupCells()
{
  int plCnt = _group->const_players().count();
  setRowCount( plCnt + 1 );
  setColumnCount( plCnt + 1 + 1 ); // 1 column for total results

  for ( int i = 0; i < rowCount(); i ++ ) {
    for ( int j = 0; j < plCnt + 1; j ++ ) {
      QTableWidgetItem *item = new QTableWidgetItem( );
      QString text;
      if ( i == j ) {
        item->setBackground( palette().brush( QPalette::Disabled,
                                              QPalette::Background ) );
        
        if ( i == 0 )  {
          text = _group->name();
        }
        item->setFlags( Qt::NoItemFlags );
      } else if ( i == 0 ) { // 0th row
        text = _group->const_players().at( j - 1 ).name();
        item->setFlags( Qt::NoItemFlags );
      } else if ( j == 0 ) { // 0th column
        Player p = _group->const_players().at( i - 1 );
        text = p.name();
        text += " (" + QString::number( p.rating(), 'f', 1 ) + ")";
        item->setFlags( Qt::NoItemFlags );
      } else {
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
      }

      item->setText( text );

      setItem( i, j, item );
    }
  
    QTableWidgetItem *item = new QTableWidgetItem( );
    if ( i == 0 ) {
      item->setText( tr( "Place" ) );
    }
    item->setFlags( Qt::NoItemFlags );
    setItem( i, plCnt + 1, item );
  }
}

/** Shows match result dialog and saves match result.
 */
void RRTable::editMatchResults( int row, int col )
{
  int aIndex = row - 1;
  int bIndex = col - 1;

  qDebug() << __FUNCTION__ << row << col;

/* tann asked to comment this.
 
  if ( _group->readOnly() ) {
    QMessageBox::information( this, _group->name(), 
                              "Group cannot be edited!" );
    qDebug() << __FUNCTION__ << "group cannot be edited!";
    return;
  }
*/
  Player a = _group->const_players().at( aIndex );
  Player b = _group->const_players().at( bIndex );
 
  Match match = _group->match( a, b );

  qDebug() << __FUNCTION__ << "edit match" << match.gamesToString();
  MatchResDialog dialog( match, this );

  if ( dialog.exec() == QDialog::Accepted ) {
    _group->setMatchResults( a, b, dialog.match().games_const() ); 

    updateMatchCell( row, col );
    updateMatchCell( col, row );
    updatePlaces( );
    adjustSize();
  }
}

/** Writes match result into specified cell.
 */
void RRTable::updateMatchCell( int row, int col )
{
  int aIndex = row - 1;
  int bIndex = col - 1;

  PlayerList players = _group->const_players();

  if ( aIndex >= players.count() || bIndex >= players.count() ) {
    qCritical() << __FUNCTION__ << "invalid row&col indexes: " << row << col;
    return;
  }

  Player a = players.at( aIndex );
  Player b = players.at( bIndex );
 
  const Match& m = _group->match( a, b );
  
  if ( m.played() ) {
    item( row, col )->setText( m.toString() );
    item( row, col )->setToolTip( m.gamesToString() );
    item( row, col )->setBackground( SPRING_GREEN1 ); 
  } else {
    item( row, col )->setText( "" );
    item( row, col )->setBackground( palette().color( QPalette::Normal, QPalette::Base ) );
    item( row, col )->setToolTip( "" );
  }
}

/** calls updateMatchCell for each match cell ;)
 */
void RRTable::updateMatchCells( )
{
  int plCnt = _group->const_players().count();

  for ( int i = 1; i < rowCount(); i ++ ) {
    for ( int j = 1; j < plCnt + 1; j ++ ) {
      if ( i != j ) {
        updateMatchCell( i, j );
      }
    }
  }
}

/** Updates places column. RRGroup should return list of players
 * which sorted by won games.
 */
void RRTable::updatePlaces()
{
  PlayerList players = _group->const_players();
  for ( int i = 0; i < players.count(); i ++ ) {
    Player p = players.at( i );
    if ( _group->playedMatchList( p ).count() > 0 ) { 
      // player already have played at least one match
      item( i + 1, players.count() + 1)
          ->setText( QString::number( _group->playerPlace( p ) ) );
    }
  } 
}

/** Shows pop-up menu over player's name when user clicks left or right mouse
 *  button.
 */
void RRTable::mousePressEvent( QMouseEvent* event )
{
  if ( ( event->button() == Qt::LeftButton ) ||
       ( event->button() == Qt::RightButton ) ) {
    QTableWidgetItem* item = itemAt( event->pos() );
    if ( item ) {
      // only items from 0th column is allowed
      if ( item->column() == 0 ) {
        // 0th row is empty, skip this. 
        if ( item->row() != 0 ) {
          // minimum group size is 2
          if ( _group->size() > 2 ) {
            openPopupMenu( item, event->globalPos() );
            return;
          }
        }
      }
    }
  }
  QTableWidget::mousePressEvent( event ); 
}

/** Creates and fills up the player menu. User can remove player,
 *  move it to another group or do something else. 
 *  \param item -- table item which contains all neccessary information 
 *                 about the player
 *  \param pos -- menu position
 */
void RRTable::openPopupMenu( QTableWidgetItem* i, QPoint pos )
{
  QMenu* menu = new QMenu( this );
  QAction* rmAction = menu->addAction( tr( "Delete from group" ) ); 
  QAction* sel = menu->exec( pos );
  if ( sel == rmAction ) {
    qDebug() << "removing player " << i->text();
    QMessageBox::StandardButton ret =     
       QMessageBox::question( this, tr( "Removing player..." ),
                              tr( "Are you sure you want to delete " ) +
                              i->text() + tr( " from group " ) + 
                              _group->name() + "?",
                              QMessageBox::Yes | QMessageBox::No );
    if ( ret == QMessageBox::Yes ) {
      int row = i->row();

      // upper row in a table contains exact player name, i.e.
      // without player's rating.
      Player p( item( 0, row )->text(), 0.0 ); 
      
      _group->removePlayer( p );
      removeRow( row );
      removeColumn( row );

      updateMatchCells();
      updatePlaces();
      qDebug() << "group size = " << _group->size();
    }
  } 
}

