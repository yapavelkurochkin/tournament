#ifndef INTEGRALSCORES__H
#define INTEGRALSCORES__H

struct IntegralScores {
  int matchesDiff;
	int gamesDiff;
	int ballsDiff;

  // for sorting purposes
  bool operator< ( const IntegralScores& s ) const
	{
	  if ( matchesDiff < s.matchesDiff ) return true;
		if ( gamesDiff < s.gamesDiff )     return true;
		if ( ballsDiff < s.ballsDiff )     return true;
	  return false;
	}

  bool operator> ( const IntegralScores& s ) const
	{
	  if ( matchesDiff > s.matchesDiff ) return true;
		if ( gamesDiff > s.gamesDiff )     return true;
		if ( ballsDiff > s.ballsDiff )     return true;
	  return false;
	}

	IntegralScores()
	: matchesDiff( 0 ),
	  gamesDiff( 0 ),
		ballsDiff( 0 ) {};
	
	IntegralScores( int mDiff, int gDiff, int bDiff )
	: matchesDiff( mDiff ),
	  gamesDiff( gDiff ),
		ballsDiff( bDiff ) {};
};

#endif // INTEGRALSCORES__H

