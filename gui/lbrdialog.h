#ifndef LBRDIALOG_H
#define LBRDIALOG_H

#include <QDialog>
#include <QComboBox>

#include "tournprops.h"

/** Shows dialog to select the kind of braking in losers bracket 
 *  after round-robin stage.
 */
class LBrDialog : public QDialog
{
  Q_OBJECT;

  public:
    LBrDialog( QWidget* parent );

    TournProps::LBrBreakType lBrBreakType() const;

  protected:
    QComboBox *combo;
};

#endif // LBRDIALOG_H 

