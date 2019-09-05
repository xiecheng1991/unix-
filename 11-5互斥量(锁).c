/*
      程序清单11-5 使用互斥量(锁)保护数据结构
      * 互斥量(锁) 使用原理 
          在访问共享资源前对互斥量进行加锁,在访问完成后释放互斥量上的锁。在对互斥量进行进行加锁以后,任何其他试图再对互斥量加锁的>  线程将会被阻塞直到当前线程释放该互斥锁。如果释放互斥锁时有多个线程阻塞,所有在该互斥锁上的阻塞线程都会被唤醒变成可运行状态。第一
  个变为运行状态的线程可以对互斥量加锁。其他线程会再次被阻塞,在这种方式下每次只有一个线程向前执行
      * 死锁的产生
          1. 线程对同一个锁加锁两次( 原理 第一次加锁后 还未释放 再次加锁时就会被阻塞 等待第一次的锁释放 )
          2. 线程A拥有持有锁1( 已经对锁1进行了加锁 ) 尝试加锁锁2, 而线程B持有锁2 尝试加锁锁1此时就会发生死锁,两个线程都无法向前,>  简单说多个线程相互持有对方所需的资源而得不到解锁
   */
  
  #include "../my_apue.h"
  #include<pthread.h>
  
  struct foo {
      int     f_count;
      pthread_mutex_t f_lock;
  };
  
  struct foo*
  foo_alloc( void ) {
      struct foo *fp;
    if ( ( fp = malloc( sizeof( struct foo ) ) ) != NULL ) {
          fp->f_count = 1;
          if ( pthread_mutex_init( &fp->f_lock, NULL ) != 0 ) {
              free( fp );
              return NULL;
          }
      }
      return fp;
  }
  
  void
  foo_hold( struct foo* fp ) {
      if ( fp == NULL ) {
          return;
      }
      pthread_mutex_lock( &fp->f_lock );
      fp->f_count++;
      pthread_mutex_unlock( &fp->f_lock );
  }
  
  void
  foo_rele( struct foo* fp ) {
      if ( fp == NULL ) {
          return;
      }
      pthread_mutex_lock( &fp->f_lock );
      if ( --fp->f_count == 0 ) {
          //last reference
          pthread_mutex_unlock( &fp->f_lock );
          pthread_mutex_destroy( &fp->f_lock );
          free( fp );
      } else {
          pthread_mutex_unlock( &fp->f_lock );
      }
  }
  
  int
main( int argc, char* argv[] ) {
      exit( 0 );
  }
