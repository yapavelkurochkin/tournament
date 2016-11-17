#include <QHash>
#include "player.h"

Player byePlayer( "bye", 0.0 );

Player::Player( )
: _rating( 0.0 )
{
}

Player::Player( QString name, double rating )
  : _name( name ),
  _rating( rating )
{

}

/* if rating is different than it compared.
 * otherwise - name (for lexical sorting) compared reversly:  
 * player with lexically bigger name should follow last.
 */
bool Player::operator< ( const Player& pl ) const
{
  if ( _rating == pl.rating() ) {
    return ( _name > pl.name() );
  } else {
    return ( _rating < pl.rating() );
  }
}

bool Player::operator> ( const Player& pl ) const
{
  if ( _rating == pl.rating() ) {
    return ( _name < pl.name() );
  } else {
    return ( _rating > pl.rating() );
  }
}

bool Player::operator== ( const Player& pl ) const
{
  return _name == pl.name();
}

uint qHash(const Player &p)
{
  return qHash( p.name() );
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

