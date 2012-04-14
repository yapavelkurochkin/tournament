#ifndef RATINGSTABLE__H
#define RATINGSTABLE__H

#include "grouptable.h"

class RatingsTable: public GroupTable 
{
  Q_OBJECT;

  public:
    RatingsTable( Group* group, QWidget* parent = NULL );

  protected:
    void setupCells();
    void updateMatchCell( int row, int col );
    void updateMatchCells( );
    void updateTotalRatings();
};

#endif // RATINGSTABLE__H 
