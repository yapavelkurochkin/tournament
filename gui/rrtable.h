#ifndef RRTABLE__H
#define RRTABLE__H

#include <QTableWidget>
#include "group.h"

class RRTable: public QTableWidget 
{
  Q_OBJECT;

  Group* _group;
  public:
    RRTable( Group* group, QWidget* parent = NULL );

  protected slots:
    void editMatchResults( int row, int col );

  protected:
    void setupCells();
    void updateMatchCell( int row, int col );
    void updateMatchCells( );
    void updatePlaces( );
};

#endif // PLAYERTABLE__H
