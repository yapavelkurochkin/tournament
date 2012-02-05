#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include "about.h"
#include "version.h"

AboutDialog::AboutDialog( QWidget* parent )
: QDialog( parent )
{
  setWindowTitle( tr( "About" ) );

  QFormLayout* l = new QFormLayout();

  l->addRow( tr( "Developed by" ), new QLabel( tr( "Pavel Kurochkin" ) ) ); 
  l->addRow( tr( "E-Mail" ), new QLabel( "pavel.kurochkin@gmail.com" ) ); 
  l->addRow( tr( "Version" ), new QLabel( gl_version ) ); 

  QDialogButtonBox* buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok );

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

  l->addRow( buttonBox );
  
  setLayout( l );
}

