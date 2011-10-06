#include "player.h"

Player::Player( )
: _rating( 0.0 )
{
}

Player::Player( QString name, double rating )
  : _name( name ),
  _rating( rating )
{

}

bool Player::operator< ( const Player& pl ) const
{
  return ( _rating < pl.rating() );
}

bool Player::operator== ( const Player& pl ) const
{
  return _name == pl.name();
}

/* serialization operators 
 */  
QDataStream &operator>>( QDataStream & s, Player &p )
{
  s >> p._name;
  s >> p._rating;

  return s;
}

QDataStream &operator<<( QDataStream & s, const Player& p )
{
  s << p._name;
  s << p._rating;

  return s;
}

