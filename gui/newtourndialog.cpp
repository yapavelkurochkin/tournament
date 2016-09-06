#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QIntValidator>
#include <QMessageBox>

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

  typeCombo = new QComboBox();
  typeCombo->addItems( QStringList()  << tr( "Round-Robin + Playoff" )
                                      << tr( "Playoff with qualification" ) );
  connect( typeCombo, SIGNAL( currentIndexChanged( int ) ), 
           this,      SLOT( typeChanged( int ) ) );
 
  typeCombo->setCurrentIndex( 0 ); 
 
  gCombo = new QComboBox();
  gCombo->addItems( QStringList()     << "2" << "4"
                                      << "8" << "16" );

  sizeCombo = new QComboBox();
  sizeCombo->addItems( QStringList()  << "2" << "4" 
                                      << "8" << "16" << "32" );

  // how many players go through w/o qualification
  noQualEdit = new QLineEdit();
  noQualEdit->setText( "1" ); 
  noQualEdit->setAlignment( Qt::AlignRight ); 
  noQualEdit->setValidator( new QIntValidator( 0, 100 ) );

  QFormLayout *fl = new QFormLayout;
  fl->addRow( tr("&Category"), catCombo );
  fl->addRow( tr("&Type"), typeCombo );
  fl->addRow( tr("&Groups"), gCombo );
  fl->addRow( tr("&Playoff"), sizeCombo );
  fl->addRow( tr("&Seeded"), noQualEdit );
  // TODO: add selector 'Do not play round-robin stage'

  table = new PlayerTable( this );
  fl->addRow( table ); 
  fl->addRow( new QLabel( tr( "(Double click on the table "
                              "to load players list)" ) ) );

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                               | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(tryToAccept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  fl->addRow( buttonBox );
  
  setLayout( fl );
  
  typeChanged( 0 );
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

unsigned int NewTournDialog::playoffSize() const
{
  return sizeCombo->currentText().toInt();
}

unsigned int NewTournDialog::skipQualification() const
{
  return noQualEdit->text().toInt();
}

// warning: I assume that index of item in combobox corresponds 
// to correct value in enum type
TournType NewTournDialog::type() const 
{
  return (TournType) typeCombo->currentIndex();
}

/** \brief Checking whether entered values are valid.
 */
void NewTournDialog::tryToAccept() 
{
  if ( (TournType) type() == QualifPlayOff ) {
    if ( (unsigned int) players().count() < playoffSize() ) {
  		QMessageBox::information( this, "Incorrect parameters",
                                      "Playoff size could not be greater\n"
                                      "than number of players" );
      return;
    }

    if ( playoffSize() < skipQualification() ) {
	  	QMessageBox::information( this, "Incorrect parameters",
                                      "Number of players skipping qualification\n"
                                      "Could not be greater than playoff size" );
      return;
    }
  
    // TODO: there can be situation with big number of players, small playoff size
    //       and small skipQual number.
  }

  accept();
}

void NewTournDialog::typeChanged( int index )
{
  switch ( (TournType) index ) {
    case RRPlayOff: 
      gCombo->setEnabled( true );
      sizeCombo->setEnabled( false );
      noQualEdit->setEnabled( false );
      break; 
    case QualifPlayOff:
      gCombo->setEnabled( false );
      sizeCombo->setEnabled( true );
      noQualEdit->setEnabled( true );
      break;
    default: 
      qCritical( "unknown tournament type selected!" );
      break;
  }
}

