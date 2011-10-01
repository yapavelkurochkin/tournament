#ifndef RRTABLE__H
#define RRTABLE__H

#include <QTableWidget>
#include "rrgroup.h"

class RRTable: public QTableWidget 
{
  Q_OBJECT;

  RRGroup* _group;
  public:
    RRTable( RRGroup* group, QWidget* parent = NULL );

  protected slots:
    void editMatchResults( int row, int col );
};

#endif // PLAYERTABLE__H
