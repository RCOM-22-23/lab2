#!/bin/bash

# configuração do IP
ifconfig eth0 up
ifconfig eth0 172.16.41.1/24
ifconfig eth0 

# route para aceder a vlan 0 a partir de eth1 de tux44
route add -net 172.16.40.0/24 gw 172.16.41.253
# route para aceder a internet a partir de eth0 de router
route add -net 172.16.1.0/24 gw 172.16.41.254
# definir RC como default router
route add default gw 172.16.41.254

# ver routes
routes -n