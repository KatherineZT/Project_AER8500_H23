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
    unsigned long data_bits = 0;

    while (request != nullptr) { 

        string s = request;
        char * p;
        data_bits = strtoul( s.c_str(), & p, 16 );

        // 4th byte is the label
        switch (data_bits & 0xFF){
            case 128: //001
                receiveRequest[requestIndex].label = 1;
                receiveRequest[requestIndex].data1 = ((data_bits >> 10) & 0x3);
                receiveRequest[requestIndex].data2 = decodeAltitude((data_bits >> 12) & 0xFFFF);
                receiveRequest[requestIndex].ssm = ((data_bits >> 29) & 0x3);
                printf("%i\n", receiveRequest[requestIndex].label);
                printf("%lu\n", data_bits);
                printf("%f\n", receiveRequest[requestIndex].data1);
                printf("%f\n", receiveRequest[requestIndex].data2);
                printf("%i\n", receiveRequest[requestIndex].ssm);
                printf("\n");
                break;
            case 64: //002
                receiveRequest[requestIndex].label = 2;
                receiveRequest[requestIndex].data1 = decodeClimbingRate(data_bits >> 10);
                receiveRequest[requestIndex].ssm = ((data_bits >> 29) & 0x3);
                printf("%i\n", receiveRequest[requestIndex].label);
                printf("%lu\n", data_bits);
                printf("%f\n", receiveRequest[requestIndex].data1);
                printf("%i\n", receiveRequest[requestIndex].ssm);
                printf("\n");
                break;
            case 192: //003
                receiveRequest[requestIndex].label = 3;
                receiveRequest[requestIndex].data1 = decodeAngle((data_bits >> 10));
                receiveRequest[requestIndex].ssm = ((data_bits >> 29) & 0x3);
                printf("%i\n", receiveRequest[requestIndex].label);
                printf("%lu\n", data_bits);
                printf("%f\n", receiveRequest[requestIndex].data1);
                printf("%i\n", receiveRequest[requestIndex].ssm);
                printf("\n");
                break;
            default:
                receiveRequest[requestIndex].label = -1;
        }

        requestIndex++;
        request = strtok(nullptr, " "); 
    } 

    return receiveRequest;
}

float A429::decodeAngle(int bits) {
    // diza unit deci spare
    float number = 0;
    number += 10*((bits >> 16) & 0x7);
    number += 1*((bits >> 12) & 0xF);
    number += 0.1*((bits >> 8) & 0xF);
    return number;
}

float A429::decodeClimbingRate(int bits) {
    // cent diza unit deci
    float number = 0;
    number += 100*((bits >> 12) & 0xF);
    number += 10*((bits >> 8) & 0xF);
    number += 1*((bits >> 4) & 0xF);
    number += 0.1*((bits) & 0xF);
    return number;
}

float A429::decodeAltitude(int bits) {
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

unsigned int A429::encodeMotorPower(float num) {

    unsigned int bits = 0;
    int currPrecision = 0x40;

    while (currPrecision >= 1) {
        if (currPrecision <= num) {
            num -= currPrecision;
            bits |= currPrecision;
        }
        currPrecision = currPrecision >> 1;
    }
    return bits << 21;
}

unsigned int A429::encodeAltitude(float num)  {
    unsigned int bits = 0;
    int currPrecision = 0x8000;

    while (currPrecision >= 1) {
        if (currPrecision <= num) {
            num -= currPrecision;
            bits |= currPrecision;
        }
        currPrecision = currPrecision >> 1;
    }
    return bits << 12;
}

unsigned int A429::encodeClimbingRate(float num)  { 
    unsigned int bits = 0;
    if (num < 0) num *= -1;

    int val = int(num / 100);
    float tmpNum = num - 100*val;
    bits |= (val << 12);

    val = int(tmpNum / 10);
    tmpNum -= val * 10;
    bits |= (int(val) << 8);

    val = int(tmpNum);
    tmpNum -= val;
    bits |= (int(val) << 4);

    val = int(tmpNum*10);
    bits |= (int(val));

    return bits << 10;
}

unsigned int A429::encodeParity(unsigned int num)  {
    unsigned int nbBits = 0;
    for (int itr = 0; itr < 32; itr++) {
        if ((num >> itr) & 1) nbBits++;
    }
    if (nbBits % 2) return (0 << 31);
    else return (1 << 31);
}

char* A429::translateSend(a429_request request)
{
    memset(sendRequest, 0, sizeof(sendRequest)); 

    unsigned int bits = 0;

    switch (request.label) {
        case 1:
            bits |= (request.ssm << 29);
            bits |= (int(request.data1) << 10);
            bits |= encodeAltitude(request.data2);
            bits |= (1 << 7);
            bits |= encodeParity(bits);
            break;
        case 2:
            bits |= (request.ssm << 29);
            bits |= encodeClimbingRate(request.data1);
            bits |= (1 << 6);
            bits |= encodeParity(bits);
            break;
        case 4:
            bits |= (request.ssm << 29);
            bits |= encodeMotorPower(request.data1);
            bits |= (1 << 5);
            bits |= encodeParity(bits);
            break;
        default:
            bits |= 0xFFFFFFFF;
            break;
    }
    sprintf(sendRequest, "%08X", bits);
    return sendRequest;
}