#!/usr/bin/python
from socket import socket, AF_INET, SOCK_DGRAM
import can
from struct import unpack_from

if __name__ == '__main__':
    
    # Dans l'exemple, je m'execute sur une MPIC mais il faut plutot me considerer
    # comme un vrai instrument CAN bus (A350 KCCU par exemple)

    # Socket UDP du client :
    # (Non realiste : l'instrument n'est pas connecte en UDP en vrai - c'est juste pour l'exemple)
    socket_udp = socket(AF_INET, SOCK_DGRAM)
    
    # Creation de l'interface CAN bus
    can0 = can.interface.Bus(bustype='socketcan', channel='can0', bitrate=500000)

    # Partie applicative de l'instrument
    while True:
      # Attente d'un evenement : reception d'un message CAN bus
      input_msg = can0.recv()

      # Decodage
      data = unpack_from(">Q", input_msg.data)
      
      # Traitement
      # En vrai, l'instrument va faire sa job a la reception de ce message
      # Pour l'exemple, on prepare et envoie plutot une reponse UDP (non realiste)
      traitement = b"A350 KCCU a recu le message " + hex(data[0])
      socket_udp.sendto(traitement, ("172.16.0.1", 5005))

    # Fermeture du socket UDP
    socket_udp.close()
