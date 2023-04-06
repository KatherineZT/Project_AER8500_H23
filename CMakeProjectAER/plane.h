#include <math.h>

enum State {
    Ground = 0,
    Climbing = 1,
    Flying = 2
};

class Plane {
  public:
    Plane();
    void update(unsigned long refreshRate);
    void setNewAltitudeTarget(int newAltitude);
    void setNewSpeedTarget(float newSpeed);
    void setNewAngleTarget(float newAngle);
    void setNewMotorSpeedTarget(float newSpeed);
    void setAltitude(int newAltitude);
    void setMotorPower(int newPower);
    void setClimbingSpeed(float newSpeed);
    void setAngle(float newAngle);
    void setState(State newState);
    // For GUI
    int  getAltitude();
    float  getClimbingSpeed();
    float  getMotorPower();
    State getState();
  private:
    int   altitude;
    int   motorPower;
    float climbingSpeed;
    float angleOfAttack;
    int   targetAltitude;
    float targetClimbingSpeed;
    float targetAngleOfAttack;
    float targetMotorSpeed;
    State state;
    static const float         FEET_PER_METER;
    static const int           SECONDS_PER_MIN;
    static const unsigned long NS_PER_SECOND;
};