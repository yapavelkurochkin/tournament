#ifndef NEWTOURNDIALOG__H
#define NEWTOURNDIALOG__H

#include <QDialog>
#include <QComboBox>

#include "playertable.h"
#include "match.h" 

class NewTournDialog : public QDialog
{
  Q_OBJECT;

  public:
    NewTournDialog( QWidget* parent );

    QString category() const;
    Match::Type matchType() const;
    unsigned int groupCount() const;
    PlayerList players() const;

  protected:
    QComboBox *matchCombo, 
              *catCombo,
              *gCombo;
    PlayerTable *table;
};

#endif // NEWTOURNDIALOG__H 

