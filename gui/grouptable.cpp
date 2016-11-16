#include <QHeaderView>
#include <QDebug>
#include "grouptable.h"

GroupTable::GroupTable( Group* group, QWidget* parent )
: QTableWidget( parent ),
  _group( group )
{
  verticalHeader()->hide();
  horizontalHeader()->hide();

  setSelectionMode( QAbstractItemView::NoSelection );

  horizontalHeader()->setStretchLastSection( true );

  setFrameStyle( QFrame::Panel | QFrame::Sunken );
}

/** Automatically adjust minimum size of table to provide
 *  readability of all cells.
 *  Should be called each time data cells are changed.
 */
void GroupTable::adjustSize( )
{
  int width = 0;
  for ( int i = 0; i < columnCount(); i ++ ) {
    width += sizeHintForColumn( i );
  }

  int height = 0;
  for ( int i = 0; i < rowCount(); i ++ ) {
    height += sizeHintForRow( i );
  }

  // ditry hack: we need some more space
  height += 4 * sizeHintForRow( 0 );

  // 5% is for inter-cell lines
  setMinimumWidth( width + width * 0.15 );
  setMinimumHeight( height + height * 0.15 );
}

