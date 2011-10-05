#include <QDebug>
#include <math.h>

#include "tournament.h"
#include "rrgroup.h"

/** Round robin games are followed first always
 */
RRGroup::RRGroup( QChar name, Tournament* t, PlayerList players )
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
  Group::addPlayer( player );
  roundRobin();
}

