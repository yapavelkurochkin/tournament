#ifndef PLAYER__H
#define PLAYER__H

#include <QString>
#include <QDataStream>

class Player {
  QString _name;
  double _rating;

  friend QDataStream &operator>>(QDataStream &, Player &);
  friend QDataStream &operator<<(QDataStream &, const Player&);

  public:
    Player( QString n, double r );
    Player( );

    QString name() const { return _name; }
    double rating() const { return _rating; }
    QString ratingAsStr() const 
		   { return QString::number( _rating, 'f', 1 ); }

    bool operator< ( const Player& pl ) const;
    bool operator== ( const Player& pl ) const;
};

#endif // PLAYER__H

