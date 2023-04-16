#include <math.h>
#include <stdio.h>

#pragma once

enum State {
    Ground = 0,
    Climbing = 1,
    Flying = 2
};

class Plane {
  public:
    Plane();
    void update(unsigned long refreshRate);
    void setAltitude(int newAltitude);
    void setClimbingSpeed(float newSpeed);
    void setAngle(float newAngle);
    void setState(State newState);
    // For GUI
    int  getAltitude();
    float getClimbingSpeed();
    float getMotorPower();
    State getState();
  private:
    int   altitude;
    int   targetAltitude;
    float motorPower;
    float climbingRate;
    float angleOfAttack;
    State state;
    bool  freeFall;
    bool  targetSet;
    static const float         FEET_PER_METER;
    static const int           SECONDS_PER_MIN;
    static const unsigned long NS_PER_SECOND;
    static const unsigned long SPEED_INCR_W_POWER;
    static const float         ACCELERATION;
    void  refreshAltitude(unsigned long refreshRate);
    void  refreshAngleAndSpeedSmooth();
};