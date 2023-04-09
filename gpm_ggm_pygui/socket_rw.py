import socket
import struct

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 8080  # The port used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

'''with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while True:
        s.send(b"60A06080 60904C40 0B000070")
        #s.send(b"0B000040 ")

        data = s.recv(1024)
        print(f"Received {data!r}")'''


def init_socket():
    s.connect((HOST, PORT))

def recv_cmd():
    print("hi")
    return 1
    

def send_cmd(attribute, value):
    data = ""
    if (attribute == 1):
        data = encodeAltitude(value)
    elif (attribute == 2):
        data = encodeClimbingSpeed(value)
    elif (attribute == 3):
        data = encodeAngle(value)
        print(data)
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

        if (currPrecision < tmpVal):
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

    tmpVal = int(val / 100)
    tmpNum = val - 100*tmpVal
    bits |= (int(tmpVal) << 12)

    tmpVal = int(tmpNum / 10)
    tmpNum -= tmpVal * 10
    bits |= (int(tmpVal) << 8)

    tmpVal = int(tmpNum)
    tmpNum -= tmpVal
    bits |= (int(tmpVal) << 4)

    tmpVal = int(tmpNum*10)
    tmpNum -= tmpVal * 0.1
    bits |= (int(tmpVal))

    data = '{0:0{1}X}'.format(bits<<2,4)
    tmpVal = val
    parity = 1
 
    while(bits!=0):   
        if((bits&1)==1):
            parity^=1
        bits>>=1

    lastByte = ""
    if parity:
        lastByte = "E"
    else :
        lastByte = "6"

    return lastByte + "0" + data + label

def encodeAngle(val):
    label = "C0"
    bits = 0

    tmpVal = int(val / 10)
    tmpNum = val - 10*tmpVal
    bits |= (int(tmpVal) << 8)

    tmpVal = int(tmpNum)
    tmpNum -= tmpVal
    bits |= (int(tmpVal) << 4)

    tmpVal = int(tmpNum*10)
    tmpNum -= tmpVal * 0.1
    bits |= (int(tmpVal) )

    data = '{0:0{1}X}'.format(bits<<2,3)
    tmpVal = val
    parity = 1
    print(data)
 
    while(bits!=0):   
        if((bits&1)==1):
            parity^=1
        bits>>=1

    lastByte = ""
    if parity:
        lastByte = "E"
    else :
        lastByte = "6"

    return lastByte + data + "00" + label