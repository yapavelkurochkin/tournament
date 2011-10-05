#include <QHeaderView>
#include <QDebug>

#include "playertable.h"
#include "rrtable.h"
#include "matchres.h"

/** Round-robin table. Represents one round-robin group of players.
 */
RRTable::RRTable( Group* group, QWidget* parent )
: QTableWidget( parent ),
  _group( group )
{
  setupCells();

  verticalHeader()->hide();
  horizontalHeader()->hide();

  connect( this, SIGNAL( cellDoubleClicked( int, int ) ),
           this, SLOT( editMatchResults( int, int ) ) );
//  resizeColumnsToContents();
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
      } else if ( i == 0 ) {
        text = _group->const_players().at( j - 1 ).name();
        item->setFlags( Qt::NoItemFlags );
      } else if ( j == 0 ) {
        text = _group->const_players().at( i - 1 ).name();
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

  item( row, col )->setText( m.toString() );
  item( row, col )->setToolTip( m.gamesToString() );
  item( row, col )->setBackground( Qt::green ); 
}

/** Updates places column. RRGroup should return list of players
 * which sorted by won games.
 */
void RRTable::updatePlaces()
{
  PlayerList players = _group->const_players();
  for ( int i = 0; i < players.count(); i ++ ) {
    Player p = players.at( i );
    qDebug() << __FUNCTION__ << i;
    if ( _group->playedMatchList( p ).count() > 0 ) { 
      // player already have played at least one match
      item( i + 1, players.count() + 1)
          ->setText( QString::number( _group->playerPlace( p ) ) );
    }
  } 
}

