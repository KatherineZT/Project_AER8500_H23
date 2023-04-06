#include "plane.h"
#include "a429_lib.h"
#include <signal.h>
#include <time.h>
#include "socket_rw.h"

class Refresher {
  public:
    Refresher(Plane* planeState, SocketRW* sender);
    ~Refresher();
  private:
    void   UpdatePlane(unsigned long refreshRate);
    void   StartPeriodicTimer();
    void   TimerThread(union sigval arg);
    SocketRW* sender;
    Plane* planeState;
    static const unsigned long INTERVAL_NS;
};