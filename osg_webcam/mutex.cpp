#include "mutex.h"

Mutex::Mutex() {
  pthread_mutexattr_t attrib;
  pthread_mutexattr_init(&attrib);
  pthread_mutexattr_settype(&attrib,PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&pthreadlock,&attrib);
}

Mutex::~Mutex() {
  pthread_mutex_destroy(&pthreadlock);
}

void Mutex::lock() {
  pthread_mutex_lock(&pthreadlock);
}

bool Mutex::trylock() {
  return (EBUSY != pthread_mutex_trylock(&pthreadlock));
}

void Mutex::unlock() {
  pthread_mutex_unlock(&pthreadlock);
}

