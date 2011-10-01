#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "polycoeff.h"

/** Program calculates the rating value earned by 
 *  player in 1 match.
 *  Match could be played as 'best of 5' or 'best of 3'.
 *
 *  param 1 -- rating of first player
 *  param 2 -- rating of second player
 *  param 3 -- scores of first player
 *  param 4 -- scores of second player
 *
 *  Program automatically detects the type of game ('best of 3'
 *  or 'best of 5')
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

  long scoresDiff = scoresA - scoresB;
  double ratingDiff = ratingB - ratingA;

  printf( "rating difference = %f\n", ratingDiff );
  printf( "scores difference = %ld\n", scoresDiff );

  struct polynom *poly;
  unsigned poly_idx = 0;
  if ( ( scoresA == 3 ) || ( scoresB == 3 ) ) {
    switch ( scoresDiff ) {
      case 3: poly_idx = 0; break; // 3:0
      case 2: poly_idx = 1; break; // 3:1
      case 1: poly_idx = 2; break; // 3:2
      case -1: poly_idx = 3; break; // 2:3
      case -2: poly_idx = 4; break; // 1:3
      case -3: poly_idx = 5; break; // 0:3
      default: 
               fprintf( stderr, "invalid match result\n" );
               break;
    }
    
    poly = &bestof5[ poly_idx ];

  } else {
    switch ( scoresDiff ) {
      case 2: poly_idx = 0; break; // 2:0
      case 1: poly_idx = 1; break; // 2:1
      case -1: poly_idx = 2; break; // 1:2
      case -2: poly_idx = 3; break; // 0:2
      default: 
               fprintf( stderr, "invalid match result\n" );
               break;
    }
    
    poly = &bestof3[ poly_idx ];
  }
 
  double resRating = polyval( poly, ratingDiff );
  printf( "%f\n", ( resRating < 0 ) ? 0.0 : resRating );

  return 0;
}

