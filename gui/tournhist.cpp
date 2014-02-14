#include <QTemporaryFile>
#include <QDebug>
#include <QDataStream>
#include "tournhist.h"
#include "tournament.h"

TournamentHistory::TournamentHistory()
 : _current( -1 )
{
}

void TournamentHistory::push( Tournament* t )
{
  push( *t );
}

void TournamentHistory::push( Tournament &t )
{
  // history rewarded
  // we need to clean last items.
  for ( int i = _current + 1; i < _history.count(); i ++ ) {
    delete _history.at( i );
  }
  _history = _history.mid( 0, _current + 1 ); 

  QTemporaryFile* f = new QTemporaryFile( );

  if ( t.save( f ) ) {
    _history << f;
  }

  _current = _history.count() - 1;
	qDebug() << __FUNCTION__ << _current << _history.count();
}

void TournamentHistory::reset( Tournament *t )
{
  foreach( QTemporaryFile *f, _history ) {
    delete f;
  }
  _history.clear();
  _current = -1;

  // save only initial tournament to history
  QTemporaryFile* f = new QTemporaryFile( );
  if ( t->save( f ) ) {
    _history << f;
    _current = 0;
  }

	qDebug() << __FUNCTION__ << _current << _history.count();
}

Tournament* TournamentHistory::reward()
{
  if ( _current < 0 ) {
    return NULL;
  }

  Tournament* t = NULL;

  _current = ( _current > 0 ) ? _current - 1 : 0;
   
  if ( _current < _history.count() ) { 
    // loading previous tournament 
    QTemporaryFile *f = _history.at( _current );
    t = Tournament::fromFile( f->fileName() );
	}

	qDebug() << __FUNCTION__ << _current << _history.count();
	return t;
}

Tournament* TournamentHistory::forward()
{
  if ( _current < 0 ) {
    return NULL;
  }
  
  Tournament* t = NULL;

  _current = ( _current < ( _history.count() - 1 ) ) 
                ? _current + 1 : _current;
   
	// loading previous tournament 
	QTemporaryFile *f = _history.at( _current );
	t = Tournament::fromFile( f->fileName() );

	qDebug() << __FUNCTION__ << _current << _history.count();

	return t;
}

