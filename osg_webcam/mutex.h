#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>
#include <errno.h>

class Mutex {

  pthread_mutex_t pthreadlock;

  public:

  Mutex();
  ~Mutex();

  void lock();
  bool trylock();
  void unlock();

};

#endif
