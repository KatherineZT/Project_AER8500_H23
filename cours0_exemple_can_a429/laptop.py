import socket

if __name__ == '__main__':
    # Etablissement d'une connexion UDP : socket(), bind()
    socket_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    socket_udp.bind(("0.0.0.0", 5005))
    
    # Etablissement d'une connexion TCP : socket(), bind(), listen(), accept()
    socket_tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket_tcp.bind(("0.0.0.0", 8888))
    socket_tcp.listen(1)
    conn_tcp, addr = socket_tcp.accept()

    # Envoi d'un message TCP a la MPIC :
    # Simule une application CAE (Windows + Ethernet) qui veut communiquer avec un instrument CAN bus (A350 KCCU)
    # On utilise la MPIC comme Interface (Ethernet <-> CAN bus)
    conn_tcp.send(b"canbus")
    
    # Attente d'un message UDP de l'instrument (de la MPIC qui le remplace) a la reception du message CAN bus
    # (Non realiste : l'instrument n'est pas connecté en UDP en vrai - c'est pour l'exemple)
    data = socket_udp.recv(1024)
    print("Donnee recue :", data.decode()) # A350 KCCU a recu le message 0xaabbccddeeffaabb
    
    # Envoi d'un message TCP a la MPIC :
    # Simule une application CAE (Windows + Ethernet) qui veut communiquer avec un instrument A429 (B737 MAX FANS MCDU)    
    # On utilise la MPIC comme Interface (Ethernet <-> A429)
    conn_tcp.send(b"a429")
    
    # Attente d'un message UDP de l'instrument (de la MPIC qui le remplace) a la reception du message A429
    # (Non realiste : l'instrument n'est pas connecté en UDP en vrai - c'est pour l'exemple)
    data = socket_udp.recv(1024)
    print("Donnee recue :", data.decode()) # B737 MAX FANS MCDU a recu le message 0b10010001100011000100010000001101


    # Fermeture des sockets
    socket_udp.close()
    conn_tcp.close()
    socket_tcp.close()
