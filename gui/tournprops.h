#ifndef TOURNPROPS__H
#define TOURNPROPS__H

#include <QString>
#include "playerlist.h"

struct TournProps {
	// value can be used as an index. 
	enum TournType {
		RRPlayOff       = 0,  // round-robin stage + playoff
		QualifPlayOff,        // qualification (optional) + playoff
		PlayOff               // simple playoff
	};

        // Loser Bracket breaking type after Round-Robin stage
	enum LBrBreakType {
		LBrThirdThird       = 0, // short: 3-3, 4-4,etc..
		LBrPlayOff               // full: playoff
	};

  TournProps();
  TournProps( PlayerList players, QString cat, 
              unsigned int playoffNum, unsigned int seededNum );
  TournProps( PlayerList players, QString cat, unsigned int rrGroupNum );

  TournType type;
  LBrBreakType lBrBreakType;
  PlayerList players;      // list of players
  QString category;        // category name

  // type == QualifPlayOff || type == PlayOff :
  unsigned int playoffNum; // how many players in playoff (power of 2)  
  unsigned int seededNum;  // how many players will skip qualification 
                           // and play in playoff directly

  // type == RRPlayOff:
  unsigned int rrGroupNum; // how many round-robin groups should be 
                           // before playoff  

  bool validate( QString& errstr ) const; // check that properties are correct
};

QDataStream &operator>>(QDataStream &s, TournProps& t );
QDataStream &operator<<(QDataStream &s, const TournProps& t );

#endif // TOURNPROPS__H

