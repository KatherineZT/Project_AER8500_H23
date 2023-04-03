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

    char requests[1024] = {0};

    while (isRunning) 
    {
        requests = socket_1->sread();

        if (requests)
        {
            for (int itr = 0; )
        }
        
    }

    delete periodicUpdater;
    return 0;
}