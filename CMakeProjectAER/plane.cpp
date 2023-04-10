#include "plane.h"

/** PLANE CLASS
 *  Holds the state of the plane
 */

const float         Plane::FEET_PER_METER     = 3.2808;
const int           Plane::SECONDS_PER_MIN    = 60;
const unsigned long Plane::NS_PER_SECOND      = 1000000000;
const unsigned long Plane::SPEED_INCR_W_POWER = 10;
const float         Plane::ACCELERATION       = 9.8;

const int   MIN_ALTITUDE = 0;
const int   MAX_ALTITUDE = 40000;
const float CRITICAL_ANGLE = 15.0;
const float MIN_ANGLE = -16.0;
const float GND_ANGLE = 0;
const float MAX_ANGLE = 16.0;
const float MIN_CLIMBING_RATE = -800.0;
const float GND_CLIMBING_RATE = 0.0;
const float MAX_CLIMBING_RATE = 800.0;
const float BASE_MOTOR_POWER = 20.0;
const float MIN_MOTOR_POWER = 5.0;
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
 * CLIMBING SPEED
 *   Range: -800 to +800
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
    this->targetSet = false;
    this->motorPower = 0;
    this->climbingRate = 0; 
    this->angleOfAttack = 0;
    this->state = Ground;
    this->freeFall = false;
}

void Plane::update(unsigned long refreshRate) {
    if (this->state == Climbing) {
        this->refreshAltitude(refreshRate);
        printf("Altitude after update: %i\n", this->altitude);
        printf("Target Altitude after update: %i\n", this->targetAltitude);
        if ((this->angleOfAttack > CRITICAL_ANGLE || this->angleOfAttack < -1*CRITICAL_ANGLE) && !freeFall) {
            printf("CHUTE LIBRE, REDRESSEZ L'AERONEF\n");
            this->freeFall = true;
            this->targetSet = false;
            this->angleOfAttack = MIN_ANGLE;
        }
        if (freeFall) {
            float tempClimbingRate = this->climbingRate;
            this->climbingRate = tempClimbingRate - ACCELERATION;
            if (this->climbingRate < MIN_CLIMBING_RATE) this->climbingRate = MIN_CLIMBING_RATE;
            if (this->altitude == MIN_ALTITUDE) {
                printf("ECRASEMENT, CONNEXION PERDUE\n");
                this->state = Ground;
            }
        } else {
            if (this->altitude == MIN_ALTITUDE) {
                this->state = Ground;
                this->angleOfAttack = GND_ANGLE;
                this->climbingRate = GND_CLIMBING_RATE;
            } else if (this->altitude == this->targetAltitude || this->altitude == MAX_ALTITUDE ||
                       this->climbingRate == GND_CLIMBING_RATE || this->angleOfAttack == GND_ANGLE) {
                this->state = Flying;
                this->angleOfAttack = GND_ANGLE;
                this->climbingRate = GND_CLIMBING_RATE;
                this->targetSet = false;
            } else if (this->targetSet) {
                this->refreshAngleAndSpeedSmooth();
            }
        }
    }
    printf("--------------------------\n");
}

void Plane::setAltitude(int newAltitude) {
    if (freeFall) freeFall=false;
    this->targetAltitude = newAltitude;
    this->targetSet = true;
    this->state = Climbing;
    this->refreshAngleAndSpeedSmooth();
    printf("SetAltitude was called.\n");
    printf("Target altitude was set to: %i\n", this->targetAltitude);
    printf("Altitude is: %i\n", this->altitude);
    printf("Climbing rate was set to: %f\n", this->climbingRate);
    printf("Angle was set to: %f\n", this->angleOfAttack);
    printf("State was set to: %i\n", this->state);
    printf("Motor power was set to: %f\n", this->motorPower);
    printf("--------------------------\n");
}

void Plane::setAngle(float newAngle) {
    if (freeFall) freeFall=false;
    this->angleOfAttack = newAngle;
    this->state = Climbing;
    this->targetSet = false;
}

void Plane::setClimbingSpeed(float newSpeed) {
    if (freeFall) freeFall=false;
    this->climbingRate = newSpeed;
    this->state = Climbing;
    this->motorPower = BASE_MOTOR_POWER + this->climbingRate/SPEED_INCR_W_POWER;
    if (this->motorPower < MIN_MOTOR_POWER) this->motorPower = MIN_MOTOR_POWER;
    this->targetSet = false;
}

void Plane::refreshAltitude(unsigned long refreshRate) {
    int previousAltitude = this->altitude;
    float speedInFeetPerSec = (this->climbingRate / SECONDS_PER_MIN) * FEET_PER_METER;
    float distanceInFeet = speedInFeetPerSec * (refreshRate / NS_PER_SECOND);
    int newAltitude = previousAltitude + static_cast<int>(round(distanceInFeet));
    printf("Previous altitude: %i\n", previousAltitude);
    printf("Speed in ft per sec: %f\n", speedInFeetPerSec);
    printf("Alt change since last update: %f\n", distanceInFeet);
    printf("New altitude: %i\n", newAltitude);
    this->altitude = newAltitude;
    if (newAltitude < MIN_ALTITUDE) this->altitude = MIN_ALTITUDE;
    else if (newAltitude > MAX_ALTITUDE) this->altitude = MAX_ALTITUDE;
}

void Plane::refreshAngleAndSpeedSmooth() {
    float delta_altitude = static_cast<float>(abs(this->targetAltitude - this->altitude));
    float dir = (this->targetAltitude - this->altitude > 1) ? 1.0 : -1.0;
    printf("delta altitude: %f\n", delta_altitude);
    if (delta_altitude > 5000) this->climbingRate = dir * MAX_CLIMBING_RATE;
    else this->climbingRate = dir * 93.93 * log(delta_altitude);
    printf("climbing rate: %f\n", this->climbingRate);
    this->motorPower = BASE_MOTOR_POWER + this->climbingRate/SPEED_INCR_W_POWER;
    if (this->motorPower < MIN_MOTOR_POWER) this->motorPower = MIN_MOTOR_POWER;
    printf("motor power: %f\n", this->motorPower);
    this->angleOfAttack = dir * expm1(delta_altitude/14770);
    printf("angle of attack: %f\n", this->angleOfAttack);
}

int Plane::getAltitude() {
    return this->altitude;
}

float Plane::getClimbingSpeed() {
    return this->climbingRate;
}

float Plane::getMotorPower() {
    return this->motorPower;
}

State Plane::getState(){
    return this->state;
}
