#include <QDebug>
#include "swissgroup.h"

/** first plays with last. second plays with pre-last. and so on.
 *  player.count() should be even.
 *  \param fromPlace -- means the best place possible for this group
 */
SwissGroup::SwissGroup( unsigned int fromPlace, Tournament* tourn,
                        unsigned int stage, PlayerList players ) 
: Group( QString( "" ), tourn, stage, players ),
  _fromPlace( fromPlace )
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

  initGroupName( );
}

void SwissGroup::initGroupName()
{
  int cnt = _players.count();
  if ( _fromPlace == 1 ) {
    if ( cnt == 2 ) {
      _name = QObject::tr( "Final" );
    } else if ( cnt == 4 ) {
      _name = QObject::tr( "1/2 Final" );
    } else if ( cnt == 8 ) {
      _name = QObject::tr( "1/4 Final" );
    }
  } else {
    _name =  QString( "%1 - %2" )
                  .arg( _fromPlace )
                  .arg( _fromPlace + cnt - 1 );
  }
}

/** Creates 2 groups: one from winners and one from loosers
 */
QList< Group* > SwissGroup::split( ) const
{
  if ( _matches.count() < 2 ) { // nothing to split
    return QList< Group* >();
  }

  PlayerList winners, loosers;
  for ( int i = 0; i < _matches.count(); i ++ ) {
    winners << _matches.at( i ).winner();
    loosers << _matches.at( i ).looser();
  }

  QList< Group* > ret;
  ret << new SwissGroup( _fromPlace, _tournament, _stage + 1, winners );
  ret << new SwissGroup( _fromPlace + _players.count() / 2, 
                         _tournament, _stage + 1, loosers );
 
  return ret; 
}

/** Serialization operators
  */
QDataStream &operator<<( QDataStream &s, const SwissGroup &g )
{
  s << dynamic_cast< const Group& >( g );
  s << g._fromPlace;

  return s;
}

QDataStream &operator>>( QDataStream &s, SwissGroup &g )
{
  s >> dynamic_cast< Group& >( g );
  s >> g._fromPlace;
  
  return s;
}

