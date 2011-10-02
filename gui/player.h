#ifndef PLAYER__H
#define PLAYER__H

#include <QString>

class Player {
  QString _name;
  double _rating;

  public:
    Player( QString n, double r );

    QString name() const { return _name; }
    double rating() const { return _rating; }

    bool operator< ( const Player& pl ) const;
    bool operator== ( const Player& pl ) const;
};

#endif // PLAYER__H

