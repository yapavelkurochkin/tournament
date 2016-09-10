#include "tournalgofactory.h"
#include "rrplayoff.h"

TournAlgo* TournAlgoFactory::algo( TournProps p ) 
{
  if ( p.type == TournProps::RRPlayOff ) {
    return new RRPlayoffAlgo( p ); 
  }

  return NULL; 
}
