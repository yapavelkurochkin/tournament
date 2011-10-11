#include <QScrollArea>
#include <QDebug>

#include "tournwidget.h"
#include "rrtable.h"
#include "swissgroup.h"
#include "swisstable.h"

/** widget contains all tables (round-robin, swiss)
 *  and enables to dinamically add group tables.
 *  removing of groups are not neccessary.
 */
TournamentWidget::TournamentWidget( Tournament* tourn, QWidget* parent )
 : QWidget( parent ),
   _tourn( tourn )
{
  QHBoxLayout* hLayout = new QHBoxLayout();
  setLayout( hLayout );

  setupLayout( hLayout );

  createTablesForGroups( );

  connect( tourn, SIGNAL( newSwissGroupCreated( SwissGroup* ) ),
           this, SLOT( newSwissGroupCreated( SwissGroup* ) ) );
}

/** initializes necessary number of columns in horizontal layout
 *  and adds vertical layout into each column.
 *  column count = number of stages in tournament
 */
void TournamentWidget::setupLayout( QHBoxLayout* hLayout )
{
  for ( unsigned int i = 0; i < _tourn->stagesCnt(); i ++ ) {
    QVBoxLayout* l = new QVBoxLayout( ); 
    hLayout->addLayout( l );

    _groupLayouts << l;
  }
}


/** scans Tournament for all groups in each stage, creates
 *  proper table and inserts it to corresponding group layout
 */
void TournamentWidget::createTablesForGroups( )
{
  for ( unsigned int gr = 0; gr < _tourn->stagesCnt(); gr ++ ) {
    QList< Group* > groups = _tourn->groupList( gr );
    // FIXME: assuming that groups are sorted in proper order?
    for ( int i = 0; i < groups.count(); i ++ ) {
      GroupTable* t;
      if ( gr == 0 ) {
        t = new RRTable( groups[ i ], this );
      } else {
        t = new SwissTable( groups[ i ], this );
      }
      
      _groupLayouts.at( gr )->addWidget( t );
    }
  }
}

void TournamentWidget::newSwissGroupCreated( SwissGroup* g )
{
  SwissTable* st = new SwissTable( g, this );
  _groupLayouts.at( g->stage() )->addWidget( st );

  // TODO: ensure that groups are sorted in proper order!
  // otherwise, games for 1-8 places can be inserted into 
  // layout after games for 9-15
}

