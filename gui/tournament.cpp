#include <math.h>
#include <QDebug>
#include <QFile>

#include "tournament.h"
#include "rrgroup.h"

Tournament::Tournament( PlayerList players, Category category,
                        Match::Type matchType, unsigned int groupSize,
                         unsigned int stagesCnt )
 : _players( players ),
   _groupSize( groupSize ),
   _stagesCnt( stagesCnt ),
   _matchType( matchType ),
   _category( category ) 
{
  _groups = new QList<Group*>[ _stagesCnt ];
  Q_CHECK_PTR( _groups );

  breakPlayers( players );
}

Tournament::Tournament( ) 
 : _groupSize( 0 ),
   _stagesCnt( 0 ),
   _matchType( Match::BestOf3 ),
   _category( M2 ) 
{
}

void Tournament::groupChanged( Group* g )
{
  if ( g->stage() == 0 ) {
    bool rrCompleted = roundRobinCompleted();
    if ( rrCompleted ) {
      // 1st stage group size.
      // stages = 4 => gs(1) = 8
      // stages = 3 => gs(1) = 4
      int gs = 1 << (_stagesCnt - 1); 

      PlayerResultsList results = roundRobinResults();

      // at first we should get first and second places from each group.
      int winnersCnt = 2*_groups[0].count();
      PlayerResultsList players;
     
      if ( gs < winnersCnt ) {
        qWarning() << "Invalid stages count" << _stagesCnt;
      }

      for ( int i = 0; ( i < results.count() ) 
                        && ( i < (int)winnersCnt ) 
                        && ( i < gs );               i ++ ) {
        players << results.takeFirst(); 
      }

      // other players should be sorted accordingly to their scores.
      qSort( results.begin(), results.end(), qGreater< PlayerResults >() );
      
      for ( int i = players.count(); i < gs; i ++ ) {
        players << results.takeFirst();
      }
  
      PlayerResultsList loosers; // other players who are loose group games.
      int nloosers = results.count();
      if ( nloosers <= 1 ) {
        nloosers = 0;
      } else {
        // finding such nloosers that log2( nloosers ) is integer.
        while ( gs != 1 ) {
          if ( nloosers >= gs ) {
            nloosers = gs;
            break;
          }
          gs = gs/2;
        } 
      }
 
      for ( int i = 0; i < nloosers; i ++ ) {
        loosers << results.takeFirst();
      }

      qDebug() << "PLAYERS";
      for ( int i = 0; i < players.count(); i ++ ) {
        qDebug() << players.at( i ).player().name() << players.at( i ).scores();
      }
      qDebug() << "LOOSERS";
      for ( int i = 0; i < loosers.count(); i ++ ) {
        qDebug() << loosers.at( i ).player().name() << loosers.at( i ).scores();
      }
    }
  }
}

/** Sorts player list and pushes cool players into first places
 * of groups. Less cool players into second places and so on.
 * Function assumes that 'players' is sorted list.
 */
void Tournament::breakPlayers( PlayerList players )
{
  int groupCnt = ceil( (double) players.count() / _groupSize );
  
  _groups[0].clear(); 

  for ( int i = 0; i < groupCnt; i ++ ) {
    _groups[0] << new RRGroup( QChar( 'A' + i ), this );
  }

  while ( players.count() ) {
    for ( int i = 0; ( i < groupCnt ) && players.count(); i ++ ) {
      Player player = players.takeLast();
     
      _groups[0][i]->addPlayer( player );
    }
  }
}

bool Tournament::roundRobinCompleted() const 
{
  for ( int i = 0; i < _groups[0].count(); i ++ ) {
    if ( ! _groups[0].at( i )->completed() ) {
      return false;
    }
  }
 
  return true;
}

/** \return player list built by cyclic principle: 1st from 1 group,
 *          1st from 2 group, 1st from Nth group; 2nd from 1 group etc...
 */
PlayerResultsList Tournament::roundRobinResults() const
{
  PlayerResultsList list;
  for ( int p = 1; p <= (int)_groupSize; p ++ ) {
    for ( int i = 0; i < _groups[0].count(); i ++ ) {
      const Group* g = _groups[0].at( i );
      if ( p <= g->size() ) {
        Player player = g->playerByPlace( p );
        list << g->playerResults( player );
      }
    }
  }

  return list;
}

/** called when app exits by the signal aboutToQuit();
 */
void Tournament::save()
{
  QFile file( "tourn.dat" );
  if ( file.open( QIODevice::WriteOnly ) ) {
    QDataStream stream( & file );
    stream << (*this);
  } 
}

/* serialization
 */
QDataStream &operator>>(QDataStream &s, Tournament& t)
{
  int mType, cat;
  
  s >> t._players >> t._groupSize >> t._stagesCnt 
    >> mType >> cat; 

  t._category = (Tournament::Category) cat;
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
        Group* g = new Group();
        s >> (*g);
        
        g->setTournament( &t );
        t._groups[i] << g; 
      }
    }   
  }
  return s;
}

QDataStream &operator<<(QDataStream &s, const Tournament& t)
{
  s << t._players << t._groupSize << t._stagesCnt 
    << (int) t._matchType << (int) t._category; 

  for ( unsigned int i = 0; i < t._stagesCnt; i ++ ) {
    int count = t._groups[ i ].count();
    s << count;
    for ( int j = 0; j < count; j ++ ) {
      const Group* g = t._groups[ i ].at( j );
      if ( i == 0 ) { // round robin stage
        const RRGroup* rrg = dynamic_cast< const RRGroup* >( g );
        s << (*rrg);
      } else {
        s << (*g);
      }
    }   
  }
  return s;
}
