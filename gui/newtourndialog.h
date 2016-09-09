#ifndef NEWTOURNDIALOG__H
#define NEWTOURNDIALOG__H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>

#include "playertable.h"
#include "match.h" 
#include "tournprops.h"

class NewTournDialog : public QDialog
{
  Q_OBJECT;

  public:
    NewTournDialog( QWidget* parent );

    TournProps tournProps() const;

  protected slots:
    void tryToAccept();
    void typeChanged( int );

  protected:
    QComboBox *matchCombo, 
              *catCombo,
              *gCombo,
              *sizeCombo,
              *typeCombo;
    QLineEdit *noQualEdit;
    PlayerTable *table;
};

#endif // NEWTOURNDIALOG__H 

