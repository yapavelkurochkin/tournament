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

TournProps NewTournDialog::tournProps() const
{
  // warning: It is assumed that index of item in combobox corresponds 
  // to correct value in enum type
  if ( (TournProps::TournType) typeCombo->currentIndex() == TournProps::RRPlayOff ) {
    return TournProps( table->playerList(), 
                       catCombo->currentText(),
                       gCombo->currentText().toUInt() );
  } else {
    return TournProps( table->playerList(), 
                       catCombo->currentText(),
                       sizeCombo->currentText().toInt(),
                       noQualEdit->text().toInt() );
  } 
}

/** \brief Checking whether entered values are valid.
 */
void NewTournDialog::tryToAccept() 
{
  QString msg;

  if ( tournProps().validate( msg ) ) {
    accept();
  } else {
	  QMessageBox::information( this, "Incorrect parameters", msg );
  }
}

void NewTournDialog::typeChanged( int index )
{
  switch ( (TournProps::TournType) index ) {
    case TournProps::RRPlayOff: 
      gCombo->setEnabled( true );
      sizeCombo->setEnabled( false );
      noQualEdit->setEnabled( false );
      break; 
    case TournProps::QualifPlayOff:
      gCombo->setEnabled( false );
      sizeCombo->setEnabled( true );
      noQualEdit->setEnabled( true );
      break;
    default: 
      qCritical( "unknown tournament type selected!" );
      break;
  }
}

