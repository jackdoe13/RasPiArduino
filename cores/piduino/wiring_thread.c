/*
  Arduino.c -  Partial implementation of the Wiring API for the Raspberry Pi
  Copyright (c) 2015 Hristo Gochkov.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "bcm2835_registers.h"

static pthread_mutex_t thread_mutexes[4];

int start_thread(void *(*fn)(void *), void * arg){
    pthread_t myThread ;
    int fd = pthread_create(&myThread, NULL, fn, arg) ;
    pthread_detach(myThread);
    return fd;
}

int create_thread(void *(*fn)(void *)){
  return start_thread(fn, NULL);
}

void lock_thread(int index){
  pthread_mutex_lock(&thread_mutexes[index]);
}

void unlock_thread(int index){
    pthread_mutex_unlock(&thread_mutexes[index]);
}

int elevate_prio(const int pri){
  struct sched_param sched ;
  memset (&sched, 0, sizeof(sched)) ;
  if (pri > sched_get_priority_max(SCHED_RR)) sched.sched_priority = sched_get_priority_max(SCHED_RR);
  else sched.sched_priority = pri ;
  return sched_setscheduler(0, SCHED_RR, &sched) ;
}
