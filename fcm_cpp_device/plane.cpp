#include "plane.h"

/** PLANE CLASS
 *  Holds the state of the plane
 */

const float         Plane::FEET_PER_METER     = 3.2808;
const int           Plane::SECONDS_PER_MIN    = 60;
const unsigned long Plane::NS_PER_SECOND      = 1000000000;
const unsigned long Plane::SPEED_INCR_W_POWER = 1000;
const float         Plane::ACCELERATION       = 9.8;

const int   MIN_ALTITUDE = 0;
const int   MAX_ALTITUDE = 40000;
const float CRITICAL_ANGLE = 15.0;
const float MIN_ANGLE = -16.0;
const float MAX_ANGLE = 16.0;
const float MIN_CLIMBING_RATE = -800.0;
const float GND_CLIMBING_RATE = 0.0;
const float MAX_CLIMBING_RATE = 800.0;
const float BASE_MOTOR_POWER = 20.0;
const float MIN_MOTOR_POWER = 0.0;
const float MAX_MOTOR_POWER = 100.0;

/**
 * @brief Plane state attributes
 * Altitude and climbing speed are the most important
 *
 * ALTITUDE
 *   Range: 0 to 40000
 *   Unit: ft
 *   Resolution: 1
 * 
 * CLIMBING SPEED (the value in this class can be negative for ease of use)
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
    this->targetAltitude = 0;
    this->motorPower = 0;
    this->climbingRate = 0; 
    this->angleOfAttack = 0;
    this->state = Ground;
    this->freeFall = false;
}

void Plane::update(unsigned long refreshRate) {
    if (this->state == Climbing) {
        if ((this->angleOfAttack > CRITICAL_ANGLE || this->angleOfAttack < -1*CRITICAL_ANGLE) && !freeFall) {
            this->freeFall = true;
            print_console("CHUTE LIBRE, REDRESSEZ L'AERONEF");
            this->angleOfAttack = MIN_ANGLE;
        }
        if (freeFall) {
            float tempClimbingRate = this->climbingRate;
            this->climbingRate = tempClimbingRate - ACCELERATION;
            if (this->climbingRate < MIN_CLIMBING_RATE) this->climbingRate = MIN_CLIMBING_RATE;
            this->refreshAltitude(refreshRate);
            if (this->altitude == MIN_ALTITUDE) {
                print_console("ECRASEMENT, CONNEXION PERDUE");
                this->state = Ground;
            }
        } else {
            this->refreshAltitude(refreshRate);
            if (this->altitude == MIN_ALTITUDE) {
                this->state = Ground;
                this->angleOfAttack = MIN_ANGLE;
                this->climbingRate = GND_CLIMBING_RATE;
            } else if (this->altitude == this->targetAltitude || this->altitude == MAX_ALTITUDE) {
                this->state = Flying;
                this->angleOfAttack = MIN_ANGLE;
                this->climbingRate = GND_CLIMBING_RATE;
            } else {
                this->refreshAngleAndSpeedSmooth();
            }
        }
    }
}

void Plane::setAltitude(int newAltitude) {
    if (freeFall) freeFall=false;
    this->targetAltitude = newAltitude;
    this->state = Climbing;
    this->refreshAngleAndSpeedSmooth();
}

void Plane::setAngle(float newAngle) {
    if (freeFall) freeFall=false;
    this->angleOfAttack = newAngle;
    this->state = Climbing;
}

void Plane::setClimbingRate(float newSpeed) {
    if (freeFall) freeFall=false;
    this->climbingRate = this->getAngleSign() * newSpeed;
    this->state = Climbing;
    this->motorPower = BASE_MOTOR_POWER + this->climbingRate/SPEED_INCR_W_POWER;
}

void Plane::refreshAltitude(unsigned long refreshRate) {
    int previousAltitude = this->altitude;
    float speedInFeetPerSec = (this->climbingRate / SECONDS_PER_MIN) * FEET_PER_METER;
    float intervalInFeet = speedInFeetPerSec * (refreshRate / NS_PER_SECOND);
    int newAltitude = previousAltitude + static_cast<int>(round(intervalInFeet));
    this->altitude = newAltitude;
    if (newAltitude < MIN_ALTITUDE) this->altitude = MIN_ALTITUDE;
    else if (newAltitude > MAX_ALTITUDE) this->altitude = MAX_ALTITUDE;
}

void Plane::refreshAngleAndSpeedSmooth() {
    int delta_altitude = this->targetAltitude - this->altitude;
    this->angleOfAttack = static_cast<float>(round(expm1(abs(delta_altitude)/14117)));
    this->motorPower = BASE_MOTOR_POWER + this->angleOfAttack/(SPEED_INCR_W_POWER/10*BASE_MOTOR_POWER);
    this->climbingRate = (this->motorPower - BASE_MOTOR_POWER) * SPEED_INCR_W_POWER;
}

int Plane::getAngleSign() {
    return (float(0) < this->angleOfAttack) - (this->angleOfAttack < float(0));
}