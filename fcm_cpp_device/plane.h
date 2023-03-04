enum State {
    Ground = 0,
    Climbing = 1,
    Flying = 2
};

class Plane {
  public:
    Plane();
    void update(unsigned long refreshRate);
    void setNewTarget(int newAltitude, float newSpeed, float newAngle);
    void setAltitude(int newAltitude);
    void setMotorPower(int newPower);
    void setClimbingRate(float newSpeed);
    void setAngle(float newAngle);
    void setState(State newState);
    // For GUI
    int  getAltitude();
    int  getClimbingRate();
    int  getMotorPower();
  private:
    int   altitude;
    int   motorPower;
    float climbingRate;
    float angleOfAttack;
    State state;
    static const float         FEET_PER_METER;
    static const int           SECONDS_PER_MIN;
    static const unsigned long NS_PER_SECOND;
};