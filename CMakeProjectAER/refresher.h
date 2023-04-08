#include "plane.h"
#include "a429_lib.h"
#include <signal.h>
#include <time.h>
#include "socket_rw.h"

class Refresher {
  public:
    Refresher(Plane* planeState, SocketRW* sender);
    ~Refresher();
    void   UpdatePlane(unsigned long refreshRate);
    static const unsigned long INTERVAL;
  private:
    void   StartPeriodicTimer();
    SocketRW* sender;
    Plane* planeState;
};

void TimerThread(union sigval arg);