#ifndef NEWTOURNDIALOG__H
#define NEWTOURNDIALOG__H

#include <QDialog>

#include "ui_newtourn.h"

class NewTournDialog : public QDialog, private Ui_Dialog
{
  Q_OBJECT;

  public:
    NewTournDialog( QWidget* parent );

};

#endif // NEWTOURNDIALOG__H 

