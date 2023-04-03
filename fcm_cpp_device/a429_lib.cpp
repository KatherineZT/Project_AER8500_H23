// do error detect in new commands here
#include "a429_lib.h"
#include <iostream>
using namespace std;

A429::A429() {
    memset(receiveRequest, 0, sizeof(receiveRequest));
    memset(sendRequest, 0, sizeof(sendRequest));
}

a429_request* A429::translateReceive(char* data)
{   
    int byteIndex = 0;
    int requestIndex = 0;
    memset(receiveRequest, 0, sizeof(receiveRequest));

    char *request = strtok(data, " "); 
    unsigned long data = 0;

    while (request != nullptr) { 

        string s = token;
        char * p;
        data = strtoul( s.c_str(), & p, 16 );
        a429_request* currRequest = &receiveRequest[requestIndex];

        // 4th byte is the label
        switch (data & 0xFF){
            case 128: //001
                currRequest->label = 1;
                currRequest->data1 = ((data >> 10) & 0x3);
                currRequest->data2 = decodeAltitude((data >> 12) & 0xFFFF);
                currRequest->ssm = ((data >> 29) & 0x3);
                break;
            case 64: //002
                currRequest->label = 2;
                currRequest->data1 = decodeClimbingRate(data >> 10);
                currRequest->ssm = ((data >> 29) & 0x3);
                break;
            case 192: //003
                currRequest->label = 3;
                currRequest->data1 = decodeAngle((data >> 10) & 0x3);
                currRequest->ssm = ((data >> 29) & 0x3);
                break;
            default:
                currRequest->label = -1;
        }

        requestIndex++;
        request = strtok(nullptr, " "); 
    } 

    return receiveRequest;
}

float A429::decodeAngle(int bits) {
    // diza unit deci spare
    float number = 0;
    number += 10*(bits >> 16) & 0xF;
    number += 1*(bits >> 12) & 0xF;
    number += 0.1*(bits >> 8) & 0xF;
    return number;
}

float A429::decodeAltitude(int bits) {
    // cent diza unit deci
    float number = 0;
    number += 100*(bits >> 16) & 0xF;
    number += 10*(bits >> 12) & 0xF;
    number += 1*(bits >> 8) & 0xF;
    number += 0.1*((bits >> 4) & 0xF);
    return number;
}

float A429::decodeClimbingRate(int bits) {
    float number = 0;
    number += 32768*((bits >> 15) & 1);
    number += 16384*((bits >> 14) & 1);
    number += 8192*((bits >> 13) & 1);
    number += 4096*((bits >> 12) & 1);
    number += 2048*((bits >> 11) & 1);
    number += 1024*((bits >> 10) & 1);
    number += 512*((bits >> 9) & 1);
    number += 256*((bits >> 8) & 1);
    number += 128*((bits >> 7) & 1);
    number += 64*((bits >> 6) & 1);
    number += 32*((bits >> 5) & 1);
    number += 16*((bits >> 4) & 1);
    number += 8*((bits >> 3) & 1);
    number += 4*((bits >> 2) & 1);
    number += 2*((bits >> 1) & 1);
    number += 1*(bits & 1);
    return number;
}

unsigned int A429::decodeAltitude(float num) {
    unsigned number = 0;
    number += 32768*((bits >> 15) & 1);
    number += 16384*((bits >> 14) & 1);
    number += 8192*((bits >> 13) & 1);
    number += 4096*((bits >> 12) & 1);
    number += 2048*((bits >> 11) & 1);
    number += 1024*((bits >> 10) & 1);
    number += 512*((bits >> 9) & 1);
    number += 256*((bits >> 8) & 1);
    number += 128*((bits >> 7) & 1);
    number += 64*((bits >> 6) & 1);
    number += 32*((bits >> 5) & 1);
    number += 16*((bits >> 4) & 1);
    number += 8*((bits >> 3) & 1);
    number += 4*((bits >> 2) & 1);
    number += 2*((bits >> 1) & 1);
    number += 1*(bits & 1);
    return number;
}

unsigned int A429::encodeAltitude(float num)  {
    unsigned int bits = 0;
    int currPrecision = 32768;

    while (currPrecision > 1) {
        if (currPrecision < num) {
            num -= currPrecision;
            bits |= currPrecision;
        }
        currPrecision /= 2;
    }
    return bits << 12;
}

unsigned int A429::encodeClimbingRate(float num)  { //YOU WERE WORKING HERE
    unsigned int bits = 0;
    int chiffre = tmpNum % 100;
    float val
    bits |= (int(tmpVal) << 16);
    tmpVal %= 10;
    bits |= (int(tmpVal) << 12);
    bits |= (int(tmpVal) << 8);
    bits |= (int(tmpVal) << 4);
    
    return bits << 12;
}

unsigned int A429::encodeAngle(float num)  { // TODO
    unsigned int bits = 0;

    return bits << 12;
}

unsigned int A429::encodeParity(float num)  { // TODO
    unsigned int bits = 0;

    return bits << 12;
}

char* A429::translateSend(a429_request request)
{
    memset(sendRequest, 0, sizeof(sendRequest)); 

    unsigned int bits = 0;

    switch (request.label) {
        case 1:
            bits |= (request.ssm << 29);
            bits |= (int(request).data1 << 10)
    }
    sprintf(sendRequest, "%X", bits);
    return sendRequest;
}