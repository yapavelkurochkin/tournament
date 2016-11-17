#include "tournalgofactory.h"
#include "rrplayoff.h"
#include "qplayoff.h"
#include "playoff.h"

TournAlgo* TournAlgoFactory::algo( TournProps p ) 
{
  if ( p.type == TournProps::RRPlayOff ) {
    return new RRPlayoffAlgo( p ); 
  } else if ( p.type == TournProps::QualifPlayOff ) { 
    return new QPlayoffAlgo( p );
  } else if ( p.type == TournProps::PlayOff ) { 
    return new PlayoffAlgo( p );
  }

  return NULL; 
}
