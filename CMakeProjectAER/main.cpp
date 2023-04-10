#include "a429_lib.h"
#include "plane.h"
#include "socket_rw.h"
#include "refresher.h"

/** MAIN
 *  This program represents the avionics system,
 *   or the FCM (flight control module).
 */

int main(int argc, char const* argv[]) {
    printf("Start\n");
    Plane* planeState = new Plane();
    SocketRW* socket_1 = new SocketRW();
    Refresher* periodicUpdater = new Refresher(planeState, socket_1);
    bool isRunning = true;
    void* requests;
    printf("Init done\n");
    while (isRunning) 
    {
        requests = socket_1->sread();
        if (requests)
        {
            a429_request* requestsPtr = (a429_request*)(requests);
            for (int itr = 0; itr < 10; itr++) {
                switch (requestsPtr[itr].label) {
                    case 1:
                        planeState->setAltitude(requestsPtr[itr].data2);
                        break;
                    case 2:
                        if (requestsPtr[itr].ssm == 3){
                            planeState->setClimbingSpeed(requestsPtr[itr].data1);
                        } else {
                            planeState->setClimbingSpeed(-1 * requestsPtr[itr].data1);
                        }
                        break;
                    case 3:
                        if (requestsPtr[itr].ssm == 3){
                            planeState->setAngle(requestsPtr[itr].data1);
                        } else {
                            planeState->setAngle(-1 * requestsPtr[itr].data1);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        
    }
    printf("App done\n");
    delete periodicUpdater;
    return 0;
}