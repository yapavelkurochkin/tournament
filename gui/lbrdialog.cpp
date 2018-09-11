#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QIntValidator>
#include <QMessageBox>

#include "lbrdialog.h"
#include "tournalgo.h"

LBrDialog::LBrDialog( QWidget* parent )
: QDialog( parent )
{
  setWindowTitle( tr( "How to break loser bracket?" ) );

  combo = new QComboBox();
  combo->addItems( QStringList()   << tr( "3-3, 4-4, ..." ) 
                                   << tr( "Playoff" ) );

  QFormLayout *fl = new QFormLayout;
  fl->addRow( tr("&Breaking"), combo );

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

  fl->addRow( buttonBox );
  
  setLayout( fl );
}

TournProps::LBrBreakType LBrDialog::lBrBreakType() const
{
  return (TournProps::LBrBreakType)combo->currentIndex();
}

