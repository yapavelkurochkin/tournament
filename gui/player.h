#ifndef PLAYER__H
#define PLAYER__H

#include <QString>
#include <QDataStream>
#include <QJsonObject>

class Player {
  QString _name;
  double _rating;

  friend QDataStream &operator>>(QDataStream &, Player &);
  friend QDataStream &operator<<(QDataStream &, const Player&);

  public:
    Player( QString n, double r = 0.0 );
    Player( );

    QString name() const { return _name; }
    double rating() const { return _rating; }
    bool isBye() const { return _name.toLower() == "bye"; }

    QString ratingAsStr() const 
		   { return QString::number( _rating, 'f', 1 ); }

    bool operator< ( const Player& pl ) const;
    bool operator> ( const Player& pl ) const;
    bool operator== ( const Player& pl ) const;

    void write(QJsonObject &json) const;
};

extern Player byePlayer;
extern uint qHash(const Player &p);

#endif // PLAYER__H

