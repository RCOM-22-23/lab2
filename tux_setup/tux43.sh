#!/bin/bash

# configuração do IP
ifconfig eth0 up
ifconfig eth0 172.16.40.1/24
ifconfig eth0 

# route para aceder a vlan 1 a partir de eth0 de tux44
route add -net 172.16.41.0/24 gw 172.16.40.254
# definir tux44 como default router
route add default gw 172.16.40.254
# ver routes
routes -n