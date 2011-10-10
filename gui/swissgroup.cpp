#include <QDebug>
#include "swissgroup.h"

/** first plays with last. second plays with pre-last. and so on.
 *  player.count() should be even.
 */
SwissGroup::SwissGroup( QString name, Tournament* tourn,
                        unsigned int stage, PlayerList players ) 
: Group( name, tourn, stage, players )
{
  int cnt = _players.count();
  if ( cnt & 0x1 ) {
    qWarning() << __FUNCTION__ << "players count should be even instead of"
               << cnt;
  }

  for ( int i = 0; i < cnt / 2; i ++ ) {
    Player a = _players.at( i );
    Player b = _players.at( cnt - 1 - i );
    qDebug() << __FUNCTION__ << a.name() << b.name(); 
    _matches << Match( a, b );
  }
}

/** Serialization operators
  */
QDataStream &operator<<( QDataStream &s, const SwissGroup &g )
{
  s << dynamic_cast< const Group& >( g );

  return s;
}

QDataStream &operator>>( QDataStream &s, SwissGroup &g )
{
  s >> dynamic_cast< Group& >( g );
  
  return s;
}

