#ifndef NEWPLAYERDIALOG__H
#define NEWPLAYERDIALOG__H

#include <QDialog>
#include "player.h"

class QLineEdit;

class NewPlayerDialog : public QDialog {
  Q_OBJECT;

  public:
    NewPlayerDialog( QWidget* parent );

    Player newPlayer() const;
  protected:
    Player _player;
    QLineEdit* _playerName;
    QLineEdit* _playerRating;
};

#endif // NEWPLAYERDIALOG__H
