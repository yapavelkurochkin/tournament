#include <QDebug>
#include "tournprops.h"

TournProps::TournProps()
: type( RRPlayOff ),
  category( "undefined" ),
  playoffNum( 0 ),
  seededNum( 0 ),
  rrGroupNum( 0 )
{
}

TournProps::TournProps( PlayerList _p, QString _c, 
                        unsigned int _pnum, unsigned int _snum )
: type( QualifPlayOff ),
  players( _p ),
  category( _c ),
  playoffNum( _pnum ),
  seededNum( _snum ),
  rrGroupNum( 0 )
{
}

TournProps::TournProps( PlayerList _p, QString _c, unsigned int _rrnum )
: type( RRPlayOff ),
  players( _p ),
  category( _c ),
  playoffNum( _rrnum * 2 ), // two players from each round-robin group
                            // will pass to playoff
  seededNum( 0 ),           // each player goes through round-robin stage 
  rrGroupNum( _rrnum )
{

}

bool TournProps::validate( QString& errtext )
{
  errtext = "";

  if ( type == QualifPlayOff ) {
    if ( (unsigned int) ( players.count() ) < playoffNum ) {
  		errtext = "Playoff size could not be greater than number of players";
      return false;
    }

    if ( playoffNum < seededNum ) {
	    errtext = "Number of players skipping qualification could "
                "not be greater than playoff size";
      return false;
    }
  
    // TODO: there can be situation with big number of players, small playoff size
    //       and small skipQual number.
  }

  return true;  
}
