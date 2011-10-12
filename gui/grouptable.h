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
  
    void adjustSize();
  public:
    GroupTable( Group* group, QWidget* parent = NULL );

    const Group* group() const { return _group; }
};


#endif // GROUPTABLE__H
