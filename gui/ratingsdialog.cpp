#include <QVBoxLayout>
#include "ratingsdialog.h"
#include "ratingstable.h"

/**
 * \param group -- group of players
 */
RatingsDialog::RatingsDialog( Group* group, QWidget* parent )
: QDialog( parent )
{
  setWindowTitle( tr( "Ratings" ) );

  RatingsTable* rt = new RatingsTable( group, this );
  QVBoxLayout* vbl = new QVBoxLayout( this );
  vbl->addWidget( rt );
  
  setLayout( vbl );
}
