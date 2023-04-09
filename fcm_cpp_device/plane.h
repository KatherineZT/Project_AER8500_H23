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
    void setClimbingRate(float newSpeed);
    void setAngle(float newAngle);
    // For GUI
    int   getAltitude();
    float getClimbingRate();
    float getMotorPower();
  private:
    int   altitude;
    int   targetAltitude;
    float motorPower;
    float climbingRate;
    float angleOfAttack;
    State state;
    bool  freeFall;
    static const float         FEET_PER_METER;
    static const int           SECONDS_PER_MIN;
    static const unsigned long NS_PER_SECOND;
    static const unsigned long SPEED_INCR_W_POWER;
    static const float         ACCELERATION;
    void  refreshAltitude(unsigned long refreshRate);
    void  refreshAngleAndSpeedSmooth();
    int   getAngleSign();
};