#include "refresher.h"

// Set the refresh rate of the system in nsec
const unsigned long Refresher::INTERVAL_NS = 1000000;

Refresher::Refresher(Plane* planeState, SocketRW* sender) {
    this->planeState = planeState;
    this->sender = sender;
    this->StartPeriodicTimer();
}

Refresher::~Refresher() {
}

void Refresher::UpdatePlane(unsigned long refreshRate) {
    this->planeState->update(refreshRate);
    // Update GUI as well
    this->sender->supdate(this->planeState->getAltitude(), this->planeState->getClimbingSpeed(), static_cast<int>(this->planeState->getState()));
}

void Refresher::StartPeriodicTimer() {
    struct sigevent sigevent;
    struct itimerspec timerspec;
    timer_t timerID;

    sigevent.sigev_notify = SIGEV_THREAD;
    sigevent.sigev_value.sival_ptr =  this;
    sigevent.sigev_notify_function = TimerThread;
    sigevent.sigev_notify_attributes = NULL;
    // Set timer expiration
    timerspec.it_value.tv_sec = 0;
    timerspec.it_value.tv_nsec = 0;
    // Set timer period
    timerspec.it_interval.tv_sec = INTERVAL_NS / 1000000000;
    timerspec.it_interval.tv_nsec = INTERVAL_NS;

    timer_create(CLOCK_REALTIME, &sigevent, &timerID);
    timer_settime(timerID, 0, &timerspec, 0);
}

void Refresher::TimerThread(union sigval arg) {
    // From signal value, get object pointer
    Refresher* objPtr = static_cast<Refresher*>(arg.sival_ptr);
    // Call an update of the avionics system
    objPtr->UpdatePlane(Refresher::INTERVAL_NS);
}
