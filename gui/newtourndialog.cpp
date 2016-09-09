#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

#include "newtourndialog.h"

NewTournDialog::NewTournDialog( QWidget* parent )
: QDialog( parent )
{
  setWindowTitle( tr( "New tournament" ) );

  catCombo = new QComboBox();
  catCombo->addItems( QStringList()   << tr( "M1" ) 
                                      << tr( "M2" )
                                      << tr( "M3" )
                                      << tr( "W" )
                                      << tr( "Elite" ) );

  gCombo = new QComboBox();
  gCombo->addItems( QStringList()     << "2" << "4"
                                      << "8" << "16" );

  QFormLayout *fl = new QFormLayout;
  fl->addRow( tr("&Category"), catCombo );
  fl->addRow( tr("&Groups"), gCombo );
  // TODO: add selector 'Do not play round-robin stage'

  table = new PlayerTable( this );
  fl->addRow( table ); 
  fl->addRow( new QLabel( tr( "(Double click on the table "
                              "to load players list)" ) ) );

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                               | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  fl->addRow( buttonBox );
  
  setLayout( fl );
}

QString NewTournDialog::category() const
{
  return catCombo->currentText();
}

unsigned int NewTournDialog::groupCount() const
{
  return gCombo->currentText().toUInt();
}

PlayerList NewTournDialog::players() const 
{
  return table->playerList(); 
}
