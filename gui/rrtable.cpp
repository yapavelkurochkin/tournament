#include <QHeaderView>
#include <QDebug>

#include "playertable.h"
#include "rrtable.h"
#include "matchres.h"

/** Round-robin table. Represents one round-robin group of players.
 */
RRTable::RRTable( RRGroup* group, QWidget* parent )
: QTableWidget( parent ),
  _group( group )
{
  setRowCount( group->const_players().count() + 1 );
  setColumnCount( group->const_players().count() + 1 );

  for ( int i = 0; i < rowCount(); i ++ ) {
    for ( int j = 0; j < columnCount(); j ++ ) {
      QTableWidgetItem *item = new QTableWidgetItem( );
      QString text;
      if ( i == j ) {
        item->setBackground( palette().brush( QPalette::Disabled,
                                              QPalette::Background ) );
        
        if ( i == 0 )  {
          text = group->name();
        }
        item->setFlags( Qt::NoItemFlags );
      } else if ( i == 0 ) {
        text = group->const_players().at( j - 1 ).name();
        item->setFlags( Qt::NoItemFlags );
      } else if ( j == 0 ) {
        text = group->const_players().at( i - 1 ).name();
        item->setFlags( Qt::NoItemFlags );
      } else {
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
      }

      item->setText( text );

      setItem( i, j, item );
    }
  }

  verticalHeader()->hide();
  horizontalHeader()->hide();

  connect( this, SIGNAL( cellDoubleClicked( int, int ) ),
           this, SLOT( editMatchResults( int, int ) ) );
//  resizeColumnsToContents();

  qDebug() << itemDelegate()->metaObject()->className();
}

void RRTable::editMatchResults( int row, int col )
{
  int aIndex = row - 1;
  int bIndex = col - 1;

  qDebug() << __FUNCTION__ << row << col;

  Player a = _group->const_players().at( aIndex );
  Player b = _group->const_players().at( bIndex );
 
  Match match = _group->match( a, b );

  qDebug() << __FUNCTION__ << "will edit match" << match.resultsAsString();
  MatchResDialog dialog( match, this );

  if ( dialog.exec() == QDialog::Accepted ) {
    Match& m = _group->match( a, b );
    m.results().clear();
    m.results() << dialog.match().results();

    item( row, col )->setText( m.scoresAsString() );
    item( row, col )->setToolTip( m.resultsAsString() );
  }
}
