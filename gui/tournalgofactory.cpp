#include "tournalgofactory.h"
#include "rrplayoff.h"
#include "qplayoff.h"

TournAlgo* TournAlgoFactory::algo( TournProps p ) 
{
  if ( p.type == TournProps::RRPlayOff ) {
    return new RRPlayoffAlgo( p ); 
  } else if ( p.type == TournProps::QualifPlayOff ) { 
    return new QPlayoffAlgo( p );
  }

  return NULL; 
}
