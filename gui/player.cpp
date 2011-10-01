#include "player.h"

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
