p  #include<stdio.h>
  #include<stdlib.h>
  #include<unistd.h>
  #include<sys/wait.h>
  #include<sys/socket.h>
  #include<sys/signal.h>
  #include<errno.h>
  #include<stdarg.h>
  #include<setjmp.h>
  
  typedef void Sigfunc( int );
  Sigfunc *signal( int, Sigfunc*  );
  
  #define MAXLINE 1024
  
  void pr_mask(const char *str)//打印当前被阻塞的信号
  {
          sigset_t sigset;
          int errno_save;
  
          errno_save = errno; /* we can be called by signal handlers */
          if (sigprocmask(0, NULL, &sigset) < 0)
                  perror("sigprocmask error");
  
          printf("mask: %s", str);
          if (sigismember(&sigset, SIGINT)) printf("SIGINT ");
          if (sigismember(&sigset, SIGQUIT)) printf("SIGQUIT ");
          if (sigismember(&sigset, SIGUSR1)) printf("SIGUSR1 ");
          if (sigismember(&sigset, SIGUSR2)) printf("SIGUSR2 ");
          if (sigismember(&sigset, SIGALRM)) printf("SIGALRM ");
  
          /* remaining signals can go here */
  
          printf("n");
          errno = errno_save;
  }
  
  void err_doit(int, int, const char *, va_list);
  
  /*
   *  * Nonfatal error related to a system call.
   *   * Print a message and return.
   *    */
          void
  err_ret(const char *fmt, ...)
  {
          va_list ap;
  
          va_start(ap, fmt);
          err_doit(1, errno, fmt, ap);
          va_end(ap);
  }
  
  
  /*
   *  * Fatal error related to a system call.
   *   * Print a message and terminate.
   *    */
          void
  err_sys(const char *fmt, ...)
  {
          va_list ap;
  
          va_start(ap, fmt);
          err_doit(1, errno, fmt, ap);
          va_end(ap);
          exit(1);
  }
  
  
  /*
   *  * Fatal error unrelated to a system call.
   *   * Error code passed as explict parameter.
   *    * Print a message and terminate.
   *     */
          void
  err_exit(int error, const char *fmt, ...)
  {
          va_list ap;
  
          va_start(ap, fmt);
          err_doit(1, error, fmt, ap);
          va_end(ap);
          exit(1);
  }
  
  
  /*
   *  * Fatal error related to a system call.
   *   * Print a message, dump core, and terminate.
   *    */
          void
  err_dump(const char *fmt, ...)
  {
          va_list ap;
  
          va_start(ap, fmt);
          err_doit(1, errno, fmt, ap);
          va_end(ap);
          abort(); /* dump core and terminate */
          exit(1); /* shouldn't get here */
  }
  
  
  /*
   *  * Nonfatal error unrelated to a system call.
   *   * Print a message and return.
   *    */
          void
  err_msg(const char *fmt, ...)
  {
          va_list ap;
  
          va_start(ap, fmt);
          err_doit(0, 0, fmt, ap);
          va_end(ap);
  }
  
  
  /*
   *  * Fatal error unrelated to a system call.
   *   * Print a message and terminate.
   *    */
          void
  err_quit(const char *fmt, ...)
  {
          va_list ap;
  
          va_start(ap, fmt);
          err_doit(0, 0, fmt, ap);
          va_end(ap);
          exit(1);
  }
  
  
  /*
   *  * Print a message and return to caller.
   *   * Caller specifies "errnoflag".
   *    */
  void
  err_doit(int errnoflag, int error, const char *fmt, va_list ap)
  {
          char buf[MAXLINE];
          vsnprintf(buf, MAXLINE, fmt, ap);
          if (errnoflag)
                snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s",
                                strerror(error));
          strcat(buf, "\n");
          fflush(stdout); /* in case stdout and stderr are the same */
          fputs(buf, stderr);
          fflush(NULL); /* flushes all stdio output streams */
  }
       
