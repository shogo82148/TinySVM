#ifndef _TIMER_H
#define _TIMER_H

// $Id: timer.h,v 1.2 2000/12/01 22:55:43 taku-ku Exp $;

class Timer
{
private:
  char buf[32];
  long start_clock;
  long end_clock;

#ifndef _WIN32
  struct tms start_tms;
  struct tms end_tms;
#endif
   
 public:
#ifdef _WIN32
  void start() { start_clock = GetTickCount()/10; };
  void end()   { end_clock   = GetTickCount()/10; };
#else
  void start() { times(&start_tms); start_clock = start_tms.tms_utime; };
  void end()   { times(&end_tms);   end_clock   = end_tms.tms_utime;   };
#endif

  char *getDiff()
  {
    this->end();
    int s  = (end_clock - start_clock) / 100;
    int m  = s / 60;
    int h  = s / 3600;
    s %= 60;
    sprintf(buf,"%d:%02d:%02d",h,m,s);
    return buf;
  }

  Timer() 
  {
    this->start();
  }
};

#endif
