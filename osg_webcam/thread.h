#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include "mutex.h"

typedef void (*func)(void *);
typedef void *(*func2)(void*);

enum ThreadStatus { NOT_RUNNING, RUNNING, STOPPING, WAITING_FOR_JOIN };

class Thread {

  ThreadStatus status;
  Mutex statuslock;
  pthread_t thread;
  void *args;

  public:
  Thread();
  ~Thread();

  ThreadStatus getStatus();
  inline void const * getArgs() const { return args; }
  bool start(func,void *args);
  void stop();
  void join();

  bool stopCheck();
  void stopNow();
  
};

#endif
