#include <math.h>
#include "polynom.h"

// see 'octave' manual (polyval and polyfit functions)
//
double polyval( const struct polynom* p, double x )
{
  int i = 0;
  double result = 0;
  for ( i = POLYNOM_COEFF_CNT - 1; i >=0; i -- ) {
    int order = ( POLYNOM_COEFF_CNT - 1 ) - i;
    result += p->p[i] * pow( ( x - p->mu[0] ) / p->mu[1], order );
  }

  return result;
}

