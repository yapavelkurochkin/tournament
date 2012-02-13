#ifndef RRTABLE__H
#define RRTABLE__H

#include "grouptable.h"

class RRTable: public GroupTable 
{
  Q_OBJECT;

  public:
    RRTable( Group* group, QWidget* parent = NULL );

  protected slots:
    void editMatchResults( int row, int col );

  protected:
    void setupCells();
    void updateMatchCell( int row, int col );
    void updateMatchCells( );
    void updatePlaces( );

    void mousePressEvent( QMouseEvent* event );
    void openPopupMenu( QTableWidgetItem* item, QPoint pos );
};

#endif // PLAYERTABLE__H
