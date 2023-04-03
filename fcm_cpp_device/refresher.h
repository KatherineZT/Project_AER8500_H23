//#include "plane.h"
#include "a429_lib.h"
//#include <signal.h>
#include <time.h>

class Refresher {
  public:
    Refresher(Plane* plane, A429* sender);
    ~Refresher();
  private:
    void   UpdatePlane(unsigned long refreshRate);
    void   StartPeriodicTimer();
    void   TimerThread(union sigval arg);
    Plane* planeState;
    A429*  sender;
    static const unsigned long INTERVAL_NS;
};