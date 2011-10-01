#ifndef POLINOM__H
#define POLINOM__H

#define POLYNOM_MAX_ORDER  3   
#define POLYNOM_COEFF_CNT  ( POLYNOM_MAX_ORDER + 1 )

struct polynom {
  double p[ POLYNOM_COEFF_CNT ];
  double mu[2];
};

double polyval( const struct polynom* p, double x );

#endif // POLINOM__H

