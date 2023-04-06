#include "plane.h"

/** PLANE CLASS
 *  Holds the state of the plane
 */

const float         Plane::FEET_PER_METER  = 3.2808;
const int           Plane::SECONDS_PER_MIN = 60;
const unsigned long Plane::NS_PER_SECOND   = 1000000000;

const int ALTITUDE_MAX = 40000;
const int ALTITUDE_MIN = 0;

/**
 * @brief Plane state attributes
 * Altitude and climbing speed are the most important
 *
 * ALTITUDE
 *   Range: 0 to 40000
 *   Unit: ft
 *   Resolution: 1
 * 
 * CLIMBING SPEED
 *   Range: 0 to 800
 *   Unit: m/min
 *   Resolution: 0.1
 * 
 * ANGLE OF ATTACK
 *   Range: -16 to +16
 *   Unit: degree
 *   Resolution: 0.1
 * 
 * MOTOR POWER
 *   Range: 0 to 100
 *   Unit: %
 *   Resolution: ?
 * 
 * STATE
 *   Values: Ground (altitude=0&&speed=0), Climbing (speed/=0) or Flying (altitude/=0&&speed=0)
 *   Data type: string or number
 * 
 */

Plane::Plane() {
    this->altitude = 0;
    this->motorPower = 0;
    this->climbingSpeed = 0; 
    this->angleOfAttack = 0;
    this->state = Ground;
}

void Plane::update(unsigned long refreshRate) {
    //1st step: check if target altitude reached
    
    int previousAltitude = this->altitude;
    //2nd step: with speed, update altitude
    float speedInFeetPerSec = (this->climbingSpeed / SECONDS_PER_MIN) * FEET_PER_METER;
    float intervalInFeet = speedInFeetPerSec * (refreshRate / NS_PER_SECOND);
    int newAltitude = previousAltitude + static_cast<int>(round(intervalInFeet));
    if ((newAltitude >= ALTITUDE_MIN) && (newAltitude <= ALTITUDE_MAX))
        this->altitude = newAltitude;
    //3rd step (not yet implemented): adjust angle if target approaching (which will adjust speed)
}

void Plane::setNewAltitudeTarget(int newAltitude) {
    this->targetAltitude = newAltitude;
}

void Plane::setNewSpeedTarget(float newSpeed) {
    this->targetClimbingSpeed = newSpeed;
}

void Plane::setNewAngleTarget(float newAngle) {
    this->targetAngleOfAttack = newAngle;
}

void Plane::setNewMotorSpeedTarget(float newSpeed) {
    this->targetMotorSpeed = newSpeed;
}

void Plane::setAltitude(int newAltitude) {
    this->altitude = newAltitude;
}

void Plane::setAngle(float newAngle) {
    this->angleOfAttack = newAngle;
    // update speed
}

void Plane::setClimbingSpeed(float newSpeed) {
    this->climbingSpeed = newSpeed;
    // update angle
}

void Plane::setMotorPower(int newPower) {
    this->motorPower = newPower;
    // update speed
}

void Plane::setState(State newState) {
    this->state = newState;
    // update speed and angle
}