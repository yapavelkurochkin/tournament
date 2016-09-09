#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "rating.h"

/** Program calculates the rating value earned by 
 *  player in 1 match.
 *  Match could be played as 'best of 5' or 'best of 3'.
 *  \sa calcEarnedRating()
 */
int main( int argc, char** argv )
{
  if ( argc != 5 ) {
    fprintf( stderr, "Usage: %s ratingA ratingB scoreA scoreB\n", argv[0] );
    return -EINVAL;
  }

  double ratingA = strtod( argv[1], NULL );
  double ratingB = strtod( argv[2], NULL );
  long scoresA = strtol( argv[3], NULL, 10 );
  long scoresB = strtol( argv[4], NULL, 10 );

  double resRating = calcEarnedRating( ratingA, ratingB,
                                       scoresA, scoresB );
  if ( isnan( resRating ) ) {
    //exit( EINVAL );
    resRating = 0;
  }

  printf( "%f\n", resRating );

  return 0;
}

