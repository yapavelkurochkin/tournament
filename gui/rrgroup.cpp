#include <QDebug>
#include <math.h>

#include "tournament.h"
#include "rrgroup.h"

/** Round robin games are followed first always
 */
RRGroup::RRGroup( QString name, Tournament* t, PlayerList players )
 : Group( name, t, 0, players ) 
{
  roundRobin();
}

void RRGroup::roundRobin()
{
  _matches.clear();

  for ( int i = 0; i < _players.count(); i ++ ) {
    for ( int j = i + 1; j < _players.count(); j ++ ) {
      qDebug() << __FUNCTION__ << _players.at( i ).name() << _players.at( j ).name(); 
      _matches << Match( _players.at( i ), _players.at( j ) );
    }
  }
}

void RRGroup::addPlayer( Player player )
{
  // note that there are some matches can be completed
  // to the moment of addition of new player. 
  for ( int i = 0; i < _players.count(); i ++ ) {
    qDebug() << __FUNCTION__ << _players.at( i ).name() << player.name(); 
    _matches << Match( _players.at( i ), player );
  }
  
  Group::addPlayer( player );
}

/** returns required matches count to play for completion of group */
unsigned int RRGroup::matchesCount( unsigned int )
{
  // todo: formula?
  return 0; 
}

/** Serialization operators
  */
QDataStream &operator<<( QDataStream &s, const RRGroup &g )
{
  s << dynamic_cast< const Group& >( g );

  return s;
}

QDataStream &operator>>( QDataStream &s, RRGroup &g )
{
  s >> dynamic_cast< Group& >( g );
  
  return s;
}

