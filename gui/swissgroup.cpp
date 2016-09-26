#include <QDebug>
#include "tournament.h"
#include "swissgroup.h"

/** first plays with last. second plays with pre-last. and so on.
 *  player.count() should be even.
 *  \param fromPlace -- means the best place possible for this group
 */
SwissGroup::SwissGroup( unsigned int fromPlace, 
                        unsigned int stage, PlayerList players ) 
: Group( QString( "" ), stage, players ),
  _fromPlace( fromPlace )
{
  int cnt = _players.count();
  if ( cnt & 0x1 ) {
    qWarning() << __FUNCTION__ << "players count should be even instead of" << cnt;
  }

  Player a,b;
  for ( int i = 0; i < cnt/2; i ++ ) {
    a = _players.at( 2*i );
    b = _players.at( 2*i + 1);

    _matches << Match( a, b );
  }

  for ( int i = 0; i < _matches.count(); i ++ ) {
    qDebug() << __FUNCTION__ << _matches.at( i ).playerA().name()
		                         << _matches.at( i ).playerB().name(); 
  }

  initGroupName( );
}

/** Should be used only during serializing/deserializing
 */
SwissGroup::SwissGroup()
: Group( QString( "" ), 2, PlayerList() ),
  _fromPlace( 1 )
{
}

void SwissGroup::initGroupName()
{
  int cnt = _players.count();
  if ( isFinal() ) {
    _name = QObject::tr( "Final" ); 
  } else if ( isHalfFinal() ) {
    _name = QObject::tr( "1/2 Final" );
  } else if ( isQuarterFinal() ) {
    _name = QObject::tr( "1/4 Final" );
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

  QList< Group* > ret;
  ret << new SwissGroup( _fromPlace, _stage + 1, winners() );
  ret << new SwissGroup( _fromPlace + _players.count() / 2, 
                         _stage + 1, loosers() );

  foreach( Group *g, ret ) {
    g->setTournData( _tournData );
  }
 
  return ret; 
}

/** returns required matches count to play for completion of group */
unsigned int SwissGroup::matchesCount( unsigned int numOfPlayers )
{
  return numOfPlayers / 2; 
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

/** print a group results into table in CSV format
 *
 * \sep -- separator character
 */
QString SwissGroup::csvResult( QChar sep ) const
{
  QString ret;
  QTextStream out( &ret );

  out << name() << sep << sep << endl;

  // number of players = row count
  for ( int i = 0; i < _matches.count(); i ++ ) {
	  Match m = _matches.at( i );
	  if ( m.played() ) {
      out << m.playerA().name() << "-" << m.playerB().name() << sep;
      out << m.toString() << endl;
    }
	}

	return ret;
}

/** Compares swiss groups by their fromPlace field.
 */
bool SwissGroup::lessThan(const Group* g1, const Group* g2)
{
  Q_ASSERT( g1 );
  Q_ASSERT( g2 ); 

  if ( g1 && g2 ) {
    const SwissGroup* sg1 = dynamic_cast< const SwissGroup* >( g1 );
    const SwissGroup* sg2 = dynamic_cast< const SwissGroup* >( g2 );

    return sg1->fromPlace() < sg2->fromPlace();
  } else {
    return false;
  }
}

