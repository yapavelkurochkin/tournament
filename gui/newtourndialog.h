#ifndef NEWTOURNDIALOG__H
#define NEWTOURNDIALOG__H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>

#include "playertable.h"
#include "match.h" 
#include "tourntype.h"

class NewTournDialog : public QDialog
{
  Q_OBJECT;

  public:
    NewTournDialog( QWidget* parent );

    QString category() const;
    unsigned int groupCount() const;
    PlayerList players() const;
    unsigned int playoffSize() const;
    unsigned int skipQualification() const;
    TournType type()  const;

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

