#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QIntValidator>
#include <QMessageBox>

#include "newtourndialog.h"
#include "tournalgo.h"
#include "tournalgofactory.h"

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
                                      << tr( "Playoff with qualification" )
                                      << tr( "Playoff" ) 
                                      << tr( "Playoff Tourn. B N.R." ) );
  connect( typeCombo, SIGNAL( currentIndexChanged( int ) ), 
           this,      SLOT( typeChanged( int ) ) );
 
  typeCombo->setCurrentIndex( 0 ); 
 
  gCombo = new QComboBox();
  gCombo->addItems( QStringList()     << "2" << "4"
                                      << "8" << "16" );

  sizeCombo = new QComboBox();
  sizeCombo->addItems( QStringList()  << "2" << "4" 
                                      << "8" << "16" << "32" << "64" );

  QFormLayout *fl = new QFormLayout;
  fl->addRow( tr("&Category"), catCombo );
  fl->addRow( tr("&Type"), typeCombo );
  fl->addRow( tr("&Groups"), gCombo );
  fl->addRow( tr("&Playoff"), sizeCombo );

  table = new PlayerTable( this );
  fl->addRow( table ); 
  fl->addRow( new QLabel( tr( "(Double click on the table "
                              "to load players list)" ) ) );

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                               | QDialogButtonBox::Cancel);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(tryToAccept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  plNumLabel = new QLabel( this );
  plNumLabel->setVisible( false );
  fl->addRow( plNumLabel ); 
  fl->addRow( buttonBox );
  
  setLayout( fl );
  
  typeChanged( 0 );

  connect( table, SIGNAL( updated( PlayerList ) ),
           this,  SLOT( playerListUpdated( ) ) );
}

TournProps NewTournDialog::tournProps() const
{
  // warning: It is assumed that index of item in combobox corresponds
  // to correct value in enum type
  TournProps::TournType type = (TournProps::TournType) typeCombo->currentIndex();
  if ( type == TournProps::RRPlayOff ) {
    return TournProps( table->playerList(),
                       catCombo->currentText(),
                       gCombo->currentText().toUInt() );
  } else {
    int pcnt = table->playerList().count();
    int size = sizeCombo->currentText().toInt();
    int seeded = pcnt;

    if ( type == TournProps::QualifPlayOff ) {
      seeded -= ( pcnt - size ) * 2;
    } 

    TournProps tp( table->playerList(),
                   catCombo->currentText(),
                   size,
                   seeded );
    tp.type = type;
    return tp;
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
      break; 
    case TournProps::QualifPlayOff:
    case TournProps::PlayOff:
    case TournProps::NRTournB:
      gCombo->setEnabled( false );
      sizeCombo->setEnabled( true );
      break;
    default: 
      qCritical( "unknown tournament type selected!" );
      break;
  }
}

void NewTournDialog::playerListUpdated( )
{
  PlayerList pl = table->playerList();
  if ( pl.count() ) {
    TournAlgo *algo = TournAlgoFactory::algo( tournProps() ); 
    if ( algo ) {
      plNumLabel->setText( "Players: "  + QString::number( pl.count() ) 
                                        + "\nMatches: "
                                        + QString::number( algo->calcMatchNum() ) );
      plNumLabel->setVisible( true );
    }
  }
}
