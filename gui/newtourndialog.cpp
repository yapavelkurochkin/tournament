#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

#include "newtourndialog.h"

NewTournDialog::NewTournDialog( QWidget* parent )
: QDialog( parent )
{
  matchCombo = new QComboBox();
  matchCombo->addItems( QStringList() << tr( "Best of 3" ) 
                                      << tr( "Best of 5" ) );

  catCombo = new QComboBox();
  catCombo->addItems( QStringList()   << tr( "M1" ) 
                                      << tr( "M2" )
                                      << tr( "M3" )
                                      << tr( "W" )
                                      << tr( "Elite" ) );

  gsCombo = new QComboBox();
  gsCombo->addItems( QStringList()    << "2" << "3"
                                      << "4" << "5"
                                      << "6" << "7" );

  QFormLayout *fl = new QFormLayout;
  fl->addRow( tr("&Match"), matchCombo );
  fl->addRow( tr("&Category"), catCombo );
  fl->addRow( tr("&Group size"), gsCombo );

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

Match::Type NewTournDialog::matchType() const
{
  return ( matchCombo->currentIndex() == 0 ) ? Match::BestOf3 : Match::BestOf5; 
}

unsigned int NewTournDialog::groupSize() const
{
  return gsCombo->currentText().toUInt();
}

PlayerList NewTournDialog::players() const 
{
  return table->playerList(); 
}
