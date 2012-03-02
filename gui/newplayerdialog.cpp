#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleValidator>

#include "newplayerdialog.h"

NewPlayerDialog::NewPlayerDialog( QWidget* parent )
: QDialog( parent )
{
  setWindowTitle( tr( "Add player" ) );

  QFormLayout* fl = new QFormLayout();
  fl->addRow( tr( "Name" ), ( _playerName = new QLineEdit( ) ) ); 
  fl->addRow( tr( "Rating" ), ( _playerRating = new QLineEdit( ) ) ); 

  _playerRating->setValidator( new QDoubleValidator( 0.0, 500.0, 2, this ) );
  
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                               | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  fl->addRow( buttonBox );
  
  setLayout( fl );
}

Player NewPlayerDialog::newPlayer() const 
{
  return Player( _playerName->text(),
                 _playerRating->text().toDouble() );
}

