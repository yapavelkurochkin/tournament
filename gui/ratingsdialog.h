#ifndef RATINGSDIALOG__H
#define RATINGSDIALOG__H

#include <QDialog>
#include "group.h"

class RatingsDialog : public QDialog
{
  Q_OBJECT;

  public:
    RatingsDialog( Group* group, QWidget* parent );
};

#endif // RATINGSDIALOG__H 

