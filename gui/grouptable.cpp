#include <QHeaderView>
#include "grouptable.h"

GroupTable::GroupTable( Group* group, QWidget* parent )
: QTableWidget( parent ),
  _group( group )
{
  verticalHeader()->hide();
  horizontalHeader()->hide();
}
