#ifndef _TIMER_H
#define _TIMER_H

// $Id: timer.h,v 1.4 2001/01/16 19:37:20 taku-ku Exp $;

namespace TinySVM {

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

  void start() 
  { 
    times(&start_tms); 
    start_clock = (start_tms.tms_utime + start_tms.tms_stime); 
  };
  
  void end()   
  { 
    times(&end_tms);
    end_clock = (end_tms.tms_utime + end_tms.tms_stime);
  };
#endif

  char *getDiff()
  {
    this->end();
    int t = (end_clock - start_clock)/100;
    int sec, min, hour;
    sec  = t % 60;
    min  = (t / 60) % 60;
    hour = t / 3600;
    sprintf(buf,"%02d:%02d:%02d", hour, min, sec);
    return buf;
  }

  Timer() 
  {
    this->start();
  }
};

};
#endif

