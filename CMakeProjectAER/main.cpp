#include "a429_lib.h"
#include "plane.h"
#include "socket_rw.h"
#include "refresher.h"

/** MAIN
 *  This program represents the avionics system,
 *   or the FCM (flight control module).
 */

int main(int argc, char const* argv[]) {

    Plane* planeState = new Plane();
    SocketRW* socket_1 = new SocketRW();
    Refresher* periodicUpdater = new Refresher(planeState, socket_1);
    bool isRunning = true;

    void* requests;

    while (isRunning) 
    {
        requests = socket_1->sread();

        if (requests)
        {
            a429_request* requestsPtr = (a429_request*)(requests);
            for (int itr = 0; itr < 10; itr++) {
                switch (requestsPtr[itr].label) {
                    case 1:
                        planeState->setNewAltitudeTarget(requestsPtr[itr].data2);
                        break;
                    case 2:
                        planeState->setNewSpeedTarget(requestsPtr[itr].data1);
                        break;
                    case 3:
                        planeState->setNewAngleTarget(requestsPtr[itr].data1);
                        break;
                    case 4:
                        planeState->setNewMotorSpeedTarget(requestsPtr[itr].data1);
                        break;
                    default:
                        break;
                }
            }
        }
        
    }

    delete periodicUpdater;
    return 0;
}