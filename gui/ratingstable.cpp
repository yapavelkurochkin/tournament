#include <QHeaderView>
#include <QDebug>
#include <QDebug>

#include "global.h"
#include "playertable.h"
#include "ratingstable.h"

/** Round-robin table. Represents one round-robin group of players.
 */
RatingsTable::RatingsTable( Group* group, QWidget* parent )
: GroupTable( group, parent ) 
{
  setupCells();
  updateMatchCells();
  updateTotalRatings();

  resizeColumnsToContents();
}

/** should be called in constructor for basic setup of 
 * table cells.
 */
void RatingsTable::setupCells()
{
  PlayerList players = _group->const_validPlayers();

  int plCnt = players.count();
  setRowCount( plCnt + 1 );
  setColumnCount( plCnt + 1 + 1 ); // 1 column for total rating results

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
        text = players.at( j - 1 ).name();
        item->setFlags( Qt::NoItemFlags );
      } else if ( j == 0 ) { // 0th column
        Player p = players.at( i - 1 );
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
      item->setText( tr( "New rating" ) );
    }
    item->setFlags( Qt::NoItemFlags );
    setItem( i, plCnt + 1, item );
  }
}

/** Writes match result and earned rating into specified cell.
 */
void RatingsTable::updateMatchCell( int row, int col )
{
  int aIndex = row - 1;
  int bIndex = col - 1;

  PlayerList players = _group->const_validPlayers();

  if ( aIndex >= players.count() || bIndex >= players.count() ) {
    qCritical() << __FUNCTION__ << "invalid row&col indexes: " << row << col;
    return;
  }

  Player a = players.at( aIndex );
  Player b = players.at( bIndex );
 
  MatchList matches = _group->matchList( a, b );
  QString text = item( row, col )->text();
  QString toolTip = item( row, col )->toolTip();
  
  foreach( Match m, matches ) {
    if ( m.played() ) {
       if ( !text.isEmpty() ) {
         text += " ";
       }
       text += m.toString();
       text += " (+" + QString().setNum( m.earnedRating( a ), 'g', 2 ) + ")";
       item( row, col )->setBackground( SPRING_GREEN1 ); 
 
       if ( !toolTip.isEmpty() ) {
         toolTip += "\n\n";
       }
       toolTip += m.gamesToString(); 
    } else {
      item( row, col )->setBackground( palette().color( QPalette::Normal, QPalette::Base ) );
    }
  }
   
  item( row, col )->setToolTip( toolTip );
  item( row, col )->setText( text );
}

/** calls updateMatchCell for each match cell ;)
 */
void RatingsTable::updateMatchCells( )
{
  PlayerList players = _group->const_validPlayers();

  int plCnt = players.count();

  for ( int i = 1; i < rowCount(); i ++ ) {
    for ( int j = 1; j < plCnt + 1; j ++ ) {
      if ( i != j ) {
        updateMatchCell( i, j );
      }
    }
  }
}

/** Update total rating for each player
 */
void RatingsTable::updateTotalRatings()
{
  PlayerList players = _group->const_validPlayers();

  int plCnt = players.count();
  int col = plCnt + 1;

  for ( int i = 1; i < rowCount(); i ++ ) {
    Player p = players.at( i - 1 );
    double earned = _group->earnedRating( p );
    double total = p.rating() + earned;    

    QString text;
    text += QString().setNum( total, 'f', 1 );
    text += " (+" + QString().setNum( earned, 'f', 1 ) + ")";
    item( i, col )->setText( text );
    item( i, col )->setBackground( SPRING_GREEN1 ); 
  }
}
