#ifndef GROUPTABLE__H
#define GROUPTABLE__H

#include <QTableWidget>
#include "group.h"

/** base class for round-robin and swiss groups visualization tables. 
 */
class GroupTable: public QTableWidget 
{
  Q_OBJECT;
  
  protected:
    Group* _group;
  public:
    GroupTable( Group* group, QWidget* parent = NULL );
};


#endif // GROUPTABLE__H
