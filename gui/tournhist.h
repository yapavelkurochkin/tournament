#ifndef TOURNHIST__H
#define TOURNHIST__H

#include <QList>

class QTemporaryFile;
class Tournament;

class TournamentHistory {
  QList< QTemporaryFile* > _history;
  int                      _current;

  public:
    TournamentHistory();

    void push( Tournament* t );
    void push( Tournament& t );
    void reset( Tournament *t );
    Tournament* reward();
    Tournament* forward();
};

#endif // TOURNHIST__H
