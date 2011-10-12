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

/** Compares swiss groups by their fromPlace field.
 */
bool swissGroupLessThen(const Group* g1, const Group* g2)
{
  const SwissGroup* sg1 = dynamic_cast< const SwissGroup* >( g1 );
  const SwissGroup* sg2 = dynamic_cast< const SwissGroup* >( g2 );

  return sg1->fromPlace() < sg2->fromPlace();
}

/** scans Tournament for all groups in each stage, creates
 *  proper table and inserts it to corresponding group layout
 */
void TournamentWidget::createTablesForGroups( )
{
  for ( unsigned int gr = 0; gr < _tourn->stagesCnt(); gr ++ ) {
    // todo: may be groupList should return sorted list?
    QList< Group* > groups = _tourn->groupList( gr );

    if ( gr > 0 ) {
      // there are swiss groups in list
      qSort( groups.begin(), groups.end(), swissGroupLessThen );
    }

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

/** creates table for group and inserts it to layout.
 */
void TournamentWidget::newSwissGroupCreated( SwissGroup* g )
{
  SwissTable* newst = new SwissTable( g, this );
  
  // because of swiss group creation context we assume that g->stage()
  // is more than 0.
  QVBoxLayout* l = _groupLayouts.at( g->stage() );

  // ensuring that groups are sorted in proper order!
  // otherwise, games for 1-8 places can be inserted into 
  // layout after games for 9-15
  int i = 0;
  for ( i = 0; i < l->count(); i ++ ) {
    QWidget* w = l->itemAt( i )->widget();
    SwissTable* st = dynamic_cast< SwissTable* >( w );
    if ( st ) {
      const SwissGroup* sg = dynamic_cast< const SwissGroup* >( st->group() );
      const SwissGroup* newsg = dynamic_cast< const SwissGroup* >( newst->group() );

      if ( sg && newsg && ( newsg->fromPlace() < sg->fromPlace() ) ) {
        break;
      }
    }
  }

  l->insertWidget( i, newst ); 
/*  if ( i < l->count() ) {
    l->insertWidget( i, newst ); 
  } else {
    l->addWidget( newst );
  }*/
}

