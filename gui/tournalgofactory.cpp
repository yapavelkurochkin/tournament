#include "tournalgofactory.h"
#include "rrplayoff.h"
#include "qplayoff.h"
#include "playoff.h"
#include "nrbplayoff.h"

TournAlgo* TournAlgoFactory::algo( TournProps p ) 
{
  if ( p.type == TournProps::RRPlayOff ) {
    return new RRPlayoffAlgo( p ); 
  } else if ( p.type == TournProps::QualifPlayOff ) { 
    return new QPlayoffAlgo( p );
  } else if ( p.type == TournProps::PlayOff ) { 
    return new PlayoffAlgo( p );
  } else if ( p.type == TournProps::NRTournB ) { 
    return new NRBPlayoffAlgo( p );
  }

  return NULL; 
}
