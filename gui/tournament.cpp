#include <math.h>
#include <QDebug>
#include <QFile>
#include <QDir>

#include "tournament.h"
#include "rrgroup.h"
#include "swissgroup.h"
#include "tournalgofactory.h"

Tournament::Tournament( TournProps props ) 
 : _magic( TOURN_MAGIC_NUMBER ),
   _algo( TournAlgoFactory::algo( props ) ),
   _data( new TournData( _algo ) )
{
  Q_CHECK_PTR( _algo );
  
  _data->setTournament( this );
  _data->initGroups();

  update(); 
}

/** This constructor is only intended for serialization/deserialization
 */
Tournament::Tournament( ) 
 : _magic( 0xdeadbeef ), // object is not valid
   _algo( NULL ),
   _data( NULL )
{
}

void Tournament::update()
{
  emit tournamentChanged( this );
}

bool Tournament::save( QString fname )
{
  QFile file( fname );
  return save( &file );
}

bool Tournament::save( QFile* f )
{
  if ( f->open( QIODevice::WriteOnly ) ) {
    QDataStream stream( f );
    stream << (*this);
    setFileName( f->fileName() );
    f->close();
    return true;
  } 

  return false;
}

/** creates an instance of Tournament initialized from specified file.
 */
Tournament* Tournament::fromFile( QString fileName )
{
  QFile file( fileName );
  Tournament* t = NULL;
  if ( file.exists() && file.open( QIODevice::ReadOnly ) ) {
    t = new Tournament();

    QDataStream ds( &file );
    
    ds >> (*t);
    t->_fileName = fileName; 
  } 

  return t;
}

/* serialization
 */
QDataStream &operator>>(QDataStream &s, Tournament& t )
{
  int mType;
  QString cat;

  if ( s.atEnd() ) {
	  return s;
	}
 
  s >> t._magic; 

  if ( t._magic != TOURN_MAGIC_NUMBER ) {
    // invalid data source. object is not valid tournament
    qWarning() << __FUNCTION__ << "invalid datastream "
    "(can't initialize Tournament object from it's contents)";
    return s;
  }
/*  
  int rrBreakAlgo = Tournament::ADBC;
  s >> t._groupCnt >> t._stagesCnt >> mType >> cat >> rrBreakAlgo; 

  t._rrBreakAlgo = (Tournament::RRBreakAlgorithm) rrBreakAlgo;
  t._category = cat;
  t._matchType = (Match::Type) mType;
  t._groups = new QList<Group*>[ t._stagesCnt ];

  for ( unsigned int i = 0; i < t._stagesCnt; i ++ ) {
    int count;
    s >> count;
    for ( int j = 0; j < count; j ++ ) {
      if ( i == 0 ) { // round robin stage
        RRGroup* rrg = new RRGroup();
        s >> (*rrg);
 
        rrg->setTournament( &t );
        t._groups[i] << rrg; 
      } else {
        SwissGroup* sg = new SwissGroup();
        s >> (*sg);
        
        sg->setTournament( &t );
        t._groups[i] << sg; 
      }
    }   
  }
*/
  return s;
}

QDataStream &operator<<(QDataStream &s, const Tournament& )
{
/*  if ( !t.isValid() ) {
    qWarning() << __FUNCTION__ << 
                  "trying to serialize invalid tournament object";
    return s;
  }
  
  s << t._magic << t._groupCnt << t._stagesCnt 
    << (int) t._matchType << t._category << (int)t._rrBreakAlgo; 

  for ( unsigned int i = 0; i < t._stagesCnt; i ++ ) {
    int count = t._groups[ i ].count();
    s << count;
    for ( int j = 0; j < count; j ++ ) {
      const Group* g = t._groups[ i ].at( j );
      if ( i == 0 ) { // round robin stage
        const RRGroup* rrg = dynamic_cast< const RRGroup* >( g );

        s << (*rrg);
      } else {
        const SwissGroup* sg = dynamic_cast< const SwissGroup* >( g );
         
        s << (*sg);
      }
    }   
  }
*/
  return s;
}

/** prints the results to csv file,
 * '|' is the separator
 */
void Tournament::saveAsCSV( QString file )
{
  if ( !isValid() ) {
    qWarning() << __FUNCTION__ << 
                  "trying to save a results of invalid tournament object";
    return;
	}

	QFile csv( file );
	QChar sep( '|' );

	if (csv.open(QFile::WriteOnly | QFile::Truncate)) {
		QTextStream out(&csv);
  
	  for ( unsigned int i = 0; i < _algo->stagesCnt(); i ++ ) {
      int count = _data->groups()[ i ].count();
      for ( int j = 0; j < count; j ++ ) {
        const Group* g = _data->groups()[ i ].at( j );
				out << g->csvResult( sep ) << endl;
      }
			out << endl;
    }

		out << totalRatingAsCSV( sep ) << endl;
  }
}

/** prints total ratings as CSV. there are 4 columns:
 *   player name, last rating, earned rating, total rating.
 *   list is sorted by last rating (from higher to lower)
 */
QString Tournament::totalRatingAsCSV( QChar sep )
{
  QString ret;
	QTextStream out( &ret );

  PlayerList pls = _data->playerList();
	qSort( pls.end(), pls.begin() );

  Group fake( "unused", _data->matchList(), pls );
  for ( int i = 0; i < pls.count(); i ++ ) {
	  Player p = pls.at( i );
    if ( !p.isBye() ) {
			double earned = fake.earnedRating( p ); 
			double total = p.rating() + earned;
			out << p.name() << sep << p.ratingAsStr() << sep
					<< "+" + QString::number( earned, 'f', 1 ) << sep
					<< QString::number( total , 'f', 1 ) << endl;
	  }
  }

	return ret;
}
