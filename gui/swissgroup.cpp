#include <QDebug>
#include "tournament.h"
#include "swissgroup.h"
#include "qplayoff.h"

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
  _fromPlace( 0 )
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
    int begin = _fromPlace;
    int end = _fromPlace + cnt - 1;
    _name =  QString( "%1 - %2" )
      .arg( begin ) // humans start counting from 1
      .arg( end );
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
  PlayerList w = winners(), l = loosers();

  // number of winners and loosers can be non-even, when group consists,
  // for example, from 6 persons. We need to add 'bye' players 
  if ( w.count() & 1 ) w << byePlayer;
  if ( l.count() & 1 ) l << byePlayer;

  qDebug() << "Winners:";
  for ( int i = 0; i < w.count(); i ++ ) {
    qDebug() << w.at( i ).name();
  }

  qDebug() << "Loosers:";
  for ( int i = 0; i < l.count(); i ++ ) {
    qDebug() << l.at( i ).name();
  }

  // winners group
  if ( !isPlayerListByed( w ) ) {
    ret << new SwissGroup( _fromPlace, _stage + 1, w ); 
  }

  // loosers group
  if ( !isPlayerListByed( l ) ) {
    ret << new SwissGroup( _fromPlace + _players.count() / 2, 
                           _stage + 1, l );
  }

  foreach( Group *g, ret ) {
    const TournAlgo *a = _tournData->algo();
    Q_CHECK_PTR( a );
 
    g->setTournData( _tournData );
    g->setQualif( isQualif() );

    // if there is at least one bye player, we should notify that
    // at least one game is fakely played.
    if ( g->const_players().contains( byePlayer ) ) {
      _tournData->groupChanged( g );
    }
  }
 
  return ret; 
}

/** returns required matches count to play for completion of group */
unsigned int SwissGroup::matchesCount( unsigned int numOfPlayers )
{
  return numOfPlayers / 2; 
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

/* redorder matches so that this group will be simply splitted by
 * winners and loosers, without permutation.
 * \warning permutePlayers should be called before this function
 * \todo this function should be in TournAlgo class... like permutePlayers()
 */
void SwissGroup::permuteMatches( BreakAlgo::Algo br )
{
  if ( _matches.count() <= 2 || br == BreakAlgo::ABCD ) {
    // nothing to permute
    return;
  }

  Q_ASSERT( ( _matches.count() & 0x01 ) == 0 );

  MatchList tmp = _matches;

  int k = 1; // k means the number of groups in one permutation entity.
             // 1 -- permuting matches
             // 2 -- permuting pairs
             // 4 -- permuting quads
  while ( ( tmp.count() / k ) > 2 ) { 
    _matches.clear();

    for ( int i = 0; i < tmp.count() / 2; i += k ) {
      if ( br == BreakAlgo::ADBC ) {  
	_matches << tmp.mid( i, k ) << tmp.mid( tmp.count() - i - k, k );
      } else if ( br == BreakAlgo::ACBD ) {
	_matches << tmp.mid( i, k ) << tmp.mid( tmp.count() / 2 + i, k );
      } 
    }
    tmp = _matches;

    k *= 2;
  }
}


/** swiss group is always defines player places in tournament.  this function
 * returns a list with paits (place-player) in tournament. Function will work
 * only if SwissGroup has size = 2 and stage > 0, because only such group
 * finally defines player's place in tournament, other wise it returns empty
 * list.  Returned list can be joined with another list and thus create full
 * list of tournament players.  returned list is unsorted! 
 */
QList< QPair< unsigned int, Player > > SwissGroup::absPlaces( )
{
  QList< QPair< unsigned int, Player > > list;
  if ( ( size() > 2 ) || ( stage() == 0 ) || !completed() ) {
    return list;
  }
  

  foreach( Player p, const_players() ) {
    unsigned int place = playerPlace( p ) - 1 + fromPlace();
    list << QPair< unsigned int, Player > ( place, p ); 
  }

  return list;
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

void SwissGroup::write( QJsonObject &json ) const
{
  json["fromPlace"] = (int)_fromPlace;
  json["toPlace"] = (int)_fromPlace + _players.count() - 1;
  json["final"] = isFinal() ? "1/1" :
                  isHalfFinal() ? "1/2" :
                  isQuarterFinal() ? "1/4": "no";

  Group::write( json );
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

