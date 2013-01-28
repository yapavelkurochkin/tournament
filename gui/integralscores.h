#ifndef INTEGRALSCORES__H
#define INTEGRALSCORES__H

struct IntegralScores {
  // total power of player. calculated as sum
	// of won matches, games and balls built with corresponding weights
  int scores;

  // for sorting purposes
  bool operator< ( const IntegralScores& s ) const
  {
	  return ( scores < s.scores );
	}

  bool operator> ( const IntegralScores& s ) const
	{
	  return ( scores > s.scores );
	}

	IntegralScores()
	: scores( 0 ) {} 
	
	IntegralScores( int mDiff, int gDiff, int bDiff )
	{ scores = mDiff * 1024 * 1024 + gDiff * 1024 + bDiff; };
};

#endif // INTEGRALSCORES__H

