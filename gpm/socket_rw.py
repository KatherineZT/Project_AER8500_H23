import socket
import struct

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 8080  # The port used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


def init_socket():
    s.connect((HOST, PORT))

def recv_cmd(window, chute_libre):
    data = (s.recv(1024)).decode("utf-8")
    data_size = len(data)
    num_packets = int(data_size / 8)
    packets = []

    for itr in range(num_packets):
        packets += [data[itr*8:itr*8+8]]

    for packet in packets:
        label = packet[6:8]
        if label == "80":
            updated_value, state = decode_altitude(packet[0:6])
            window.find_element('alt').Update(updated_value)
            window.find_element('sta').Update(state)
            if (updated_value == 0 and chute_libre == True):
                window.find_element('cons').Update("ÉCRASEMENT, CONNEXION PERDUE")
        elif label == "40":
            updated_value = decode_climbing_speed(packet[0:6])
            window.find_element('clb').Update(updated_value)
        elif label == "20":
            updated_value = decode_motor_power(packet[0:6])
            window.find_element('mtr').Update(updated_value)
        else:
            continue

    return 1

def decode_altitude(packet):
    bits_alt = int(packet[1:5],16)
    bits_state = int(packet[5:6],16)

    number = 0
    state = ""

    number += 32768*((bits_alt >> 15) & 1)
    number += 16384*((bits_alt >> 14) & 1)
    number += 8192*((bits_alt >> 13) & 1)
    number += 4096*((bits_alt >> 12) & 1)
    number += 2048*((bits_alt >> 11) & 1)
    number += 1024*((bits_alt >> 10) & 1)
    number += 512*((bits_alt >> 9) & 1)
    number += 256*((bits_alt >> 8) & 1)
    number += 128*((bits_alt >> 7) & 1)
    number += 64*((bits_alt >> 6) & 1)
    number += 32*((bits_alt >> 5) & 1)
    number += 16*((bits_alt >> 4) & 1)
    number += 8*((bits_alt >> 3) & 1)
    number += 4*((bits_alt >> 2) & 1)
    number += 2*((bits_alt >> 1) & 1)
    number += 1*(bits_alt & 1)

    if (bits_state == 0):
        state = "AU_SOL"
    elif (bits_state == 4):
        state = "CHANGEMENT_ALTITUDE"
    elif (bits_state == 8):
        state = "VOL_CROISIERE"
    else:
        state = "AU_SOL"
    
    return int(number), state

def decode_climbing_speed(packet):
    bits = int(packet[1:6],16) >> 2
    sign = int(packet[0],16) & 6

    number = 0
    number += 100*((bits >> 12) & 0xF)
    number += 10*((bits >> 8) & 0xF)
    number += 1*((bits >> 4) & 0xF)
    number += 0.1*((bits) & 0xF)

    if sign != 6:
        number *= -1

    return number

def decode_motor_power(packet):
    number = 0
    bits= int(packet[1:5],16) >> 9

    number += 64*((bits >> 6) & 1)
    number += 32*((bits >> 5) & 1)
    number += 16*((bits >> 4) & 1)
    number += 8*((bits >> 3) & 1)
    number += 4*((bits >> 2) & 1)
    number += 2*((bits >> 1) & 1)
    number += 1*(bits & 1)

    return number
    

def send_cmd(attribute, value):
    data = ""
    if (attribute == 1):
        data = encodeAltitude(value)
    elif (attribute == 2):
        data = encodeClimbingSpeed(value)
    elif (attribute == 3):
        data = encodeAngle(value)
    else:
        return -1
    
    s.send(bytes(data, 'utf-8'))

    return 1

def encodeAltitude(val):
    label = "80"
    bits = 0
    currPrecision = 32768
    tmpVal = val

    while (currPrecision >= 1):
        if (currPrecision <= tmpVal):
            tmpVal -= currPrecision
            bits |= int(currPrecision)

        currPrecision = currPrecision / 2

    data = '{0:0{1}X}'.format(bits,4)
    tmpVal = val
    parity = 1
 
    while(val!=0):   
        if((val&1)==1):
            parity^=1
        val>>=1

    lastByte = ""
    if parity:
        lastByte = "6"
    else :
        lastByte = "E"

    return lastByte + data + "0" + label

def encodeClimbingSpeed(val):
    label = "40"
    bits = 0
    is_negative = False
    if val < 0:
        is_negative = True
        val *= -1

    tmpVal = int(val / 100)
    tmpNum = val - 100*tmpVal
    bits |= (int(tmpVal) << 12)

    tmpVal = int(tmpNum / 10)
    tmpNum -= tmpVal * 10
    bits |= (int(tmpVal) << 8)

    tmpVal = int(tmpNum)
    tmpNum -= tmpVal
    bits |= (int(tmpVal) << 4)

    tmpNum += 0.01
    tmpVal = int(tmpNum*10)
    bits |= (int(tmpVal))

    data = '{0:0{1}X}'.format(bits<<2,5)
    tmpVal = val
    parity = 1
 
    while(bits!=0):   
        if((bits&1)==1):
            parity^=1
        bits>>=1

    lastByte = ""
    if parity and not is_negative:
        lastByte = "E"
    elif parity and is_negative:
        lastByte = "8"
    elif not parity and is_negative:
        lastByte = "0"
    else :
        lastByte = "6"

    return lastByte + data + label

def encodeAngle(val):
    label = "C0"
    bits = 0

    is_negative = False
    if val < 0:
        is_negative = True
        val *= -1

    tmpVal = int(val / 10)
    tmpNum = val - 10*tmpVal
    bits |= (int(tmpVal) << 8)

    tmpVal = int(tmpNum)
    tmpNum -= tmpVal
    bits |= (int(tmpVal) << 4)

    tmpNum += 0.01
    tmpVal = int(tmpNum*10)
    bits |= (int(tmpVal) )

    data = '{0:0{1}X}'.format(bits<<2,3)
    tmpVal = val
    parity = 1
 
    while(bits!=0):   
        if((bits&1)==1):
            parity^=1
        bits>>=1

    lastByte = ""
    if parity and not is_negative:
        lastByte = "E"
    elif parity and is_negative:
        lastByte = "8"
    elif not parity and is_negative:
        lastByte = "0"
    else :
        lastByte = "6"

    return lastByte + data + "00" + label