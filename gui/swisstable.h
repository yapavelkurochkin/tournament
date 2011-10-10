#ifndef SWISSTABLE__H
#define SWISSTABLE__H

#include "grouptable.h"

class SwissTable: public GroupTable 
{
  Q_OBJECT;

  public:
    SwissTable( Group* group, QWidget* parent = NULL );

  protected slots:
    void editMatchResults( int row, int col );

  protected:
    void setupCells();
    void updateMatchCell( int row, int col );
    void updateMatchCells( );
};

#endif 
