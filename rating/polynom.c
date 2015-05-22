#include <math.h>
#include "polynom.h"

#define X_MIN  -40.0
#define X_MAX  40.0

// see 'octave' manual (polyval and polyfit functions)
// 
// see also: polyval.octave and data tables bestof3, bestof5.
// function calculates value in the range specified by that tables (X_MIN..X_MAX).
//
// if value is out of range then it is saturated.
double polyval( const struct polynom* p, double x )
{
  if ( x < X_MIN ) x = X_MIN;
  if ( x > X_MAX ) x = X_MAX;

  int i = 0;
  double result = 0;
  for ( i = POLYNOM_COEFF_CNT - 1; i >=0; i -- ) {
    int order = ( POLYNOM_COEFF_CNT - 1 ) - i;
    result += p->p[i] * pow( ( x - p->mu[0] ) / p->mu[1], order );
  }

  return result;
}

