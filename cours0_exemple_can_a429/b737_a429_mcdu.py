#!/usr/bin/python
from socket import socket, AF_INET, SOCK_DGRAM
from cae_a429 import a429_channel_input

if __name__ == '__main__':
    
    # Dans l'exemple, je m'execute sur une MPIC mais il faut plutot me considerer
    # comme un vrai instrument A429 (B737 MAX FANS MCDU par exemple)

    # Socket UDP du client :
    # (Non realiste : l'instrument n'est pas connecte en UDP en vrai - c'est juste pour l'exemple)
    socket_udp = socket(AF_INET, SOCK_DGRAM)
    
    # Creation de l'interface A429
    a429_chan = a429_channel_input(1)

    # Partie applicative de l'instrument
    while True:
      # Attente d'un evenement : reception d'un mot A429
      word = a429_chan.recv()
      
      # Traitement 
      # En vrai, l'instrument va faire sa job a la reception de ce mot
      # Pour l'exemple, on prepare et envoie plutot une reponse UDP (non realiste)
      traitement = b"B737 MAX FANS MCDU a recu le message " + bin(word)
      socket_udp.sendto(traitement, ("172.16.0.1", 5005))

    # Fermeture du socket UDP
    socket_udp.close()
    # Fermeture du channel A429
    a429_chan.close()
    
