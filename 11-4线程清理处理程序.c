  /*
    unix环境高级编程 程序清单11-4 线程清理处理程序
    通过下面示例代码展示
    如果线程是通过从它的启动例程中返回而中止的话,那么它的清理处理程序就不会被调用,还要注意清理处理程序是按照与他们安装时相反的顺序被调用
   和进程的atexit函数的处理方式有点类似
  */
  #include"../my_apue.h"
  #include<pthread.h>
  
  void
cleanup( void* arg ) {
    printf( "cleanup:%s\n", ( char* arg ) );
  }
  
  void *
  thr_fn1( void* arg ) {
      printf( "thread 1 start\n" );
    pthread_cleanup_push( cleanup, "thread 1 first handler" );
    pthread_cleanup_push( cleanup, "pthread 1 second handler" );
      printf( "thread 1 push compelete\n" );
      if ( arg ) {
          return ( void* )1;
      }
  
      pthread_cleanup_pop( 0 );
      pthread_cleanup_pop( 0 );
      return ( void* )1;
  }       
         
  void *
  thr_fn2( void* arg ) {
      printf( "thread 2 start \n " );
    pthread_cleanup_push( cleanup, "pthread 2 first handler" );
    pthread_cleanup_push( cleanup, "pthread 2 second handler" );
  
      printf( "thread 2 push compelete\n" );
    
      if ( arg ) {
          pthread_exit( ( void* ) 2 );
      }
    
      pthread_cleanup_pop( 0 );
      pthread_cleanup_pop( 0 );
      pthread_exit( ( void* )2 );
  }
  
  int
main( int argc, char* argv[] ) {
      int err;
      pthread_t tid1, tid2;
      void* tret;
  
      err = pthread_create( &tid1, NULL, thr_fn1, ( void* )1 );
      if ( err != 0 ) {
        err_quit( "can't create thread 1 :%s\n", strerror( err ) );
      }
  
      err = pthread_create( &tid2, NULL, thr_fn2, ( void* )1  );
      if ( err != 0 ) {
        err_quit( "can't create thread 2 :%s\n", strerror( err ) );
      }
  
      err = pthread_join( tid1, &tret );
      if ( err != 0 ) {
        err_quit( "can't join thread 1 :%s\n", strerror( err ) );
      }
  
    err = pthread_join( tid2, &tret )
  
      exit( 0 );
  }
