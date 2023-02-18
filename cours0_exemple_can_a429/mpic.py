#!/usr/bin/python
from socket import socket, AF_INET, SOCK_STREAM
import can
from cae_a429 import a429_channel_output

if __name__ == '__main__':

    # Creation d'un socket TCP et connexion au serveur (handshaking)
    # Simule le lien entre la simulation CAE (Windows + Ethernet) et l'interface MPIC
    socket_tcp = socket(AF_INET, SOCK_STREAM)
    socket_tcp.connect(("172.16.0.1", 8888))

    # Creation de l'interface CAN bus
    # La MPIC est physiquement connectee a l'instrument (vrai protocole requis).
    can0 = can.interface.Bus(bustype='socketcan', channel='can0', bitrate=500000)

    # Creation de l'interface A429
    # La MPIC est physiquement connectee a l'instrument (vrai protocole requis).
    a429_chan = a429_channel_output(1)

    # Attente d'un message TCP (de la simulation CAE vers l'instrument CAN bus - la MPIC fait l'interface)
    msg = socket_tcp.recv(1024) # canbus

    # Envoi d'un message sur can0 (Interface MPIC <-> Instrument CAN bus)
    if msg == b"canbus":
      can0.send(can.Message(data=[0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB]))

    # Attente d'un message TCP (de la simulation CAE vers l'instrument A429 - la MPIC fait l'interface)
    msg = socket_tcp.recv(1024) # a429

    # Envoi d'un message A429 (Interface MPIC <-> Instrument A429)
    if msg == b"a429":
      # Preparation et envoi du mot
      parity = 0b1
      ssm = 0b00
      data = 0b1000110001100010001
      sdi = 0b00
      label = 0b00001101

      word = (parity << 31) + (ssm << 29) + (data << 10) + (sdi << 8) + (label << 0)
      a429_chan.send(word)

    # Fermeture du socket TCP
    socket_tcp.close()
    # Fermeture du channel A429
    a429_chan.close()
