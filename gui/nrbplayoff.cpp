#include <QDebug>
#include "nrbplayoff.h"
#include "swissgroup.h"
#include "tournament.h"

extern unsigned int log2( unsigned int x );

NRBPlayoffAlgo::NRBPlayoffAlgo( TournProps p )
: PlayoffAlgo( p )
{
}

/**
 *  List of players is divided by 4 cluster.
 *  1st cluster stays unchanged (sorted by rating). 
 *  2nd, 3rd and 4th clusters are randomized.
 */
QList<Group*> NRBPlayoffAlgo::initGroups( ) const
{
  QList< Group* > groups;
  
  PlayerList in = props_const().players;
  qSort( in.begin(), in.end(), qGreater< Player >() );

  int size = props_const().playoffNum; 
  int clSize = size/4; // cluster size = 25 %
    
  // first cluster goes through "as is"
  PlayerList pl = in.mid(0, clSize);

  // remaining clusters are randomized.
  for ( int i = clSize; i < in.count(); i += clSize ) {
    int start = i;
    int remain = in.count() - i;
    int cnt = remain > clSize ? clSize : remain;
   
    pl << rndPermPlayerList( in.mid( start, cnt ) );
  } 

  SwissGroup *sg = new SwissGroup( 1, 0, permutePlayers( pl ) );
  sg->setQualif( false );
  sg->permuteMatches( breakAlgo() );

  groups << sg;

  return groups;
}


