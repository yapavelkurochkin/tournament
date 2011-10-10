#include <QDebug>

#include "swisstable.h"
#include "matchres.h"

/** Round-robin table. Represents one round-robin group of players.
 */
SwissTable::SwissTable( Group* group, QWidget* parent )
: GroupTable( group, parent ) 
{
  setupCells();

  connect( this, SIGNAL( cellDoubleClicked( int, int ) ),
           this, SLOT( editMatchResults( int, int ) ) );

  updateMatchCells();
  
  resizeColumnsToContents( );
}

/** should be called in constructor for basic setup of 
 * table cells.
 */
void SwissTable::setupCells()
{
  MatchList matches = _group->matchList();

  setRowCount( matches.count() + 1 );
  setColumnCount( 2 ); // 1 for player names and 1 for match results

  for ( int i = 0; i < rowCount(); i ++ ) {
    for ( int j = 0; j < columnCount(); j ++ ) {
      QTableWidgetItem *item = new QTableWidgetItem( );
      QString text;
        
      if ( i == 0 ) {
        if ( j == 0 ) {
          text = _group->name();
        } 
        
        item->setFlags( Qt::NoItemFlags );
      } else {
        Match m = matches.at( i - 1 );
        if ( j == 0 ) {
          text = m.playerA().name() + " - " + m.playerB().name();
          item->setFlags( Qt::NoItemFlags );
        } else {
          item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
          updateMatchCell( i, j );
        }
      }

      item->setText( text );

      setItem( i, j, item );
    }
  }
}

/** Shows match result dialog and saves match result.
 */
void SwissTable::editMatchResults( int row, int col )
{
  int mIndex = row - 1;

  Match m = _group->matchList().at( mIndex );
  MatchResDialog dialog( m, this );

  if ( dialog.exec() == QDialog::Accepted ) {
    _group->setMatchResults( dialog.match() ); 

    updateMatchCell( row, col );
  }
}

/** Writes match result into specified cell.
 */
void SwissTable::updateMatchCell( int row, int col )
{
  int mIndex = row - 1;

  MatchList matches = _group->matchList();

  if ( mIndex >= matches.count() ) {
    qCritical() << __FUNCTION__ << "invalid row indexe: " << row;
    return;
  }

  const Match m = matches.at( mIndex );
  
  if ( m.played() ) {
    item( row, col )->setText( m.toString() );
    item( row, col )->setToolTip( m.gamesToString() );
    item( row, col )->setBackground( Qt::green ); 
  }
}

/** calls updateMatchCell for each math cell ;)
 */
void SwissTable::updateMatchCells( )
{
  for ( int i = 1; i < rowCount(); i ++ ) {
    updateMatchCell( i, 1 );
  }
}

