# AER8500-Poly
Exemple du cours AER8500

Commandes à exécuter sur la MPIC pour activer et configurer can0 et can1 :
```
SpiRequest -d 2 10c 40
ip link set can0 up type can bitrate 500000
ip link set can1 up type can bitrate 500000
```

Filtre pour Wireshark :
```
udp.port==5005 || tcp.port==8888
```