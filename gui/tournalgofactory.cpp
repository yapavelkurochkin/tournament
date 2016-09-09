#include "tournalgofactory.h"

static TournAlgo* TournAlgoFactory::algo( TournProps p ) 
{
  if ( p.type == TournProps::RRPlayOff ) {
    return new RRPlayOff( p ); 
  }

  return NULL; 
}
