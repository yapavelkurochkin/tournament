#include <math.h>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

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

    if ( !t->isValid() ) { // tried to load old .trn file
      delete t;
      return NULL;
    }

    t->_fileName = fileName; 
  } 

  return t;
}

/* serialization from stream
 */
QDataStream &operator>>(QDataStream &s, Tournament& t )
{
  if ( s.atEnd() ) {
	  return s;
	}
 
  s >> t._magic; 

  qDebug() << __FUNCTION__ << "magic=" << t._magic;

  if ( t._magic != TOURN_MAGIC_NUMBER ) {
    // invalid data source. object is not valid tournament
    qWarning() << __FUNCTION__ << "invalid datastream "
    "(can't initialize Tournament object from datastream contents)";
    return s;
  }

  TournProps props;
  s >> props; 
 
  QString errtext;
  if ( !props.validate( errtext ) ) {
    qWarning() << __FUNCTION__ << "invalid tourn props:" << errtext;
    return s;
  }
  
  qDebug() << __FUNCTION__ << "props player count=" << props.players.count();

  t._algo = TournAlgoFactory::algo( props );
  if ( !t._algo ) {
    qWarning() << "unable to load tournament (algo = NULL)";
    return s;
  }

  t._data = new TournData( t._algo );
  t._data->setTournament( &t );

  qDebug() << __FUNCTION__ << "t._data = " << (long) t._data << "t._algo =" << (long)t._algo;

  s >> (*t._data);

  qDebug() << __FUNCTION__ << (int)props.type << t._algo->stagesCnt() << 
              t._data->groups()[0].count();
  

  return s;
}

QDataStream &operator<<(QDataStream &s, const Tournament& t )
{
  if ( !t.isValid() || !t._algo || !t._data ) {
    qWarning() << __FUNCTION__ << 
                  "trying to serialize invalid tournament object";
    return s;
  }
  
  s << t._magic << t._algo->props() << (*t._data);

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

/** prints the results to .json file
 */
void Tournament::saveAsJson( QString file )
{
  if ( !isValid() ) {
    qWarning() << __FUNCTION__ << 
                  "trying to save a results of invalid tournament object";
    return;
	}

  QJsonObject json;
  json["version"] = (int)1;
  
  // players list sort accordingly to places
  QJsonArray plArray;
  QList< QPair< unsigned int, Player > > res = _data->results();
  
  for ( int i = 0; i < res.count(); i ++ ) {
		QJsonObject plObj;
	  plObj["name"] = res.at(i).second.name();
    plObj["place"] = (int)res.at(i).first;	
		plArray.append(plObj);
  }

  json["players"] = plArray;


	QFile jsonFile( file );

	if (jsonFile.open(QFile::WriteOnly | QFile::Truncate)) {
    QJsonArray gArray;
	
    for ( unsigned int i = 0; i < _algo->stagesCnt(); i ++ ) {
      QJsonObject gObj;
      int count = _data->groups()[ i ].count();
      for ( int j = 0; j < count; j ++ ) {
        const Group* g = _data->groups()[ i ].at( j );

        g->write( gObj );
        gArray.append( gObj );
      }
    }

    json["groups"] = gArray;   
 
    QJsonDocument saveDoc(json);
    jsonFile.write( saveDoc.toJson() );
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
