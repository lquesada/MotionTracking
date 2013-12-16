#include "thread.h"

Thread::Thread() {
  status = NOT_RUNNING;
}

Thread::~Thread() {
  
}

ThreadStatus Thread::getStatus() { 
  statuslock.lock();
  return status;
  statuslock.unlock();
}

bool Thread::start(func a,void *arg) {
  int rc;
  bool retval;
  pthread_attr_t attr;
  statuslock.lock();
  retval = true;
  rc = -1;
  try {
    if (status != NOT_RUNNING) {
      throw 0;
    }
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    args = arg;
    status = RUNNING;
    rc = pthread_create(&thread,&attr,(func2)a,(void *)this);
    if (rc != 0) {
      status = NOT_RUNNING;
    }
  } catch (int e) {
    retval = false;
  }
  statuslock.unlock();
  return retval&(rc==0);
}

void Thread::stop() {
  statuslock.lock();
  if (status == RUNNING) {
    status = STOPPING;
  }
  statuslock.unlock();
}

void Thread::join() {
  statuslock.lock();
  if (status == RUNNING) {
    status = STOPPING;
  }
  statuslock.unlock();
  pthread_join(thread,NULL);
  statuslock.lock();
  status = NOT_RUNNING;
  statuslock.unlock();
}

bool Thread::stopCheck() {
  if (status == STOPPING) {
    statuslock.lock();
    status = WAITING_FOR_JOIN;
    statuslock.unlock();
  }
  if (status != RUNNING)
    return false;
  return true;
}

void Thread::stopNow() {
  if (status == STOPPING || status == RUNNING) {
    statuslock.lock();
    status = WAITING_FOR_JOIN;
    statuslock.unlock();
    pthread_exit(NULL);
  }
}
