#include <QHeaderView>
#include <QDebug>
#include "playertable.h"
#include "rrtable.h"

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
}

void RRTable::editMatchResults( int row, int col )
{
  qDebug() << __FUNCTION__ << row << col;  
}
