#include <math.h>
#include <stdio.h>
#include <string.h>

struct a429_request {
    int label;
    float data1;
    float data2;
    int ssm;
};

class A429 {
  public:
    A429();
    a429_request translateReceive(char* data);
    char* translateSend(a429_request request);
  private:
    a429_request receiveRequest[10];
    char sendRequest[9];
    float decodeAltitude(int bits);
    float decodeClimbingRate(int bits);
    float decodeAngle(int bits);
    int encodeParity(int bits);
    unsigned int encodeAltitude(float num);
    unsigned int encodeClimbingRate(float num);
    unsigned int encodeAngle(float num);
};