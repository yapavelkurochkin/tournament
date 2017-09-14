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
    Player( QString n, double r = 0.0 );
    Player( );

    QString name() const { return _name; }
    double rating() const { return _rating; }
    bool isBye() const { return _name.toLower() == "bye"; }

    // Player can hold two names delimited by '/' character.
    // It means that player is not defined yet.
    bool isUndefined() const { return _name.contains("/"); }

    QString ratingAsStr() const 
		   { return QString::number( _rating, 'f', 1 ); }

    bool operator< ( const Player& pl ) const;
    bool operator> ( const Player& pl ) const;
    bool operator== ( const Player& pl ) const;
   
    static Player mkUndefinedPlayer( Player a, Player b );
};

extern Player byePlayer;
extern uint qHash(const Player &p);

#endif // PLAYER__H

