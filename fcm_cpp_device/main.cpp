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
    Refresher* periodicUpdater = new Refresher(planeState);

    delete periodicUpdater;
    return 0;
}