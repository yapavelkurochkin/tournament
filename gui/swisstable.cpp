#include <QDebug>
#include <QMessageBox>
#include "swisstable.h"
#include "matchres.h"
#include "global.h"

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
  adjustSize();
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
        item->setBackground( palette().brush( QPalette::Disabled,
                                              QPalette::Background ) );
      } else {
        Match m = matches.at( i - 1 );
        if ( j == 0 ) {
          text = m.playerA().name() + " - " + m.playerB().name();
          item->setFlags( Qt::NoItemFlags );
        } else {
          item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        }
      }

      item->setText( text );

      setItem( i, j, item );
    }
  }

  // name cell is spanned
  setSpan( 0, 0, 1, 2 );
}

/** Shows match result dialog and saves match result.
 */
void SwissTable::editMatchResults( int row, int col )
{
  int mIndex = row - 1;

  Match m = _group->matchList().at( mIndex );
  MatchResDialog dialog( m, this );

  if ( dialog.exec() == QDialog::Accepted ) {
      if (_group->readOnly()) {  // For read only groups, only allow edits if the match winner outcome will not change
          Match editedMatch = dialog.match();

          if (!(m.winner() == editedMatch.winner())) {
              QMessageBox::information( this, _group->name(),
                                        "You cannot change the match winner on a finished group." );
              qDebug() << __FUNCTION__ << "group cannot be edited (winner would change)!";
              return;
          }
      }

    _group->setMatchResults( dialog.match() ); 

    updateMatchCell( row, col );
    adjustSize();
  }
}

/** Writes match result into specified cell.
 */
void SwissTable::updateMatchCell( int row, int col )
{
  int mIndex = row - 1;

  MatchList matches = _group->matchList();

  if ( mIndex >= matches.count() ) {
    qCritical() << __FUNCTION__ << "invalid row index: " << row;
    return;
  }

  Match m = matches.at( mIndex );
  
  if ( m.played() ) {
    item( row, col )->setText( m.toString() );
    item( row, col )->setToolTip( m.gamesToString() );
    item( row, col )->setBackground( SPRING_GREEN1 ); 

    resizeColumnsToContents();
    adjustSize();
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

