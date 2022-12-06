# Recomecar conexões

```
systemctl restart networking -> Tux
/system reset-configuration -> Switch/Router
```

# Ligacoes fisicas

```
TUX42S0 -> Switch/Router Porta Consola

tux44E0  -> Switch Porta 1 
tux43E0  -> Switch Porta 2
tux42E0  -> Switch Porta 3
tux44E1  -> Switch Porta 4
routerE1 -> 4.1
routerE2 -> Switch Porta 5
```
# Configurar ip's nos terminais localmente

## terminal 4 (172.16.40.254 -> 00:21:5a:5a:7b:ea)
```
# configuração de IP's
ifconfig eth0 up
ifconfig eth0 172.16.40.254/24
ifconfig eth0 

ifconfig eth1 up
ifconfig eth1 172.16.41.253/24
ifconfig eth1 

# Enable IP forwarding
echo 1 > /proc/sys/net/ipv4/ip_forward
# Disable ICMP echo-ignore-broadcast
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

# route para aceder a internet a partir de eth0 de router
route add -net 172.16.1.0/24 gw 172.16.41.254
# definir RC como default router
route add default gw 172.16.41.254
# ver routes
routes -n

```
## terminal 3 (172.16.40.1 -> 00:21:5a:61:2f:d4)
```
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
```

## terminal 2 (172.16.41.1/24)
```
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
```

# Switch

## Adicionar bridges

```
> interface bridge add name = bridge40
> interface bridge add name = bridge41
```
## Remover ports de ether da bridge default

```
> interface bridge port remove [find interface = ether1]
> interface bridge port remove [find interface = ether2]
> interface bridge port remove [find interface = ether3]
> interface bridge port remove [find interface = ether4]
> interface bridge port remove [find interface = ether5]
```

## Adicionar tux 2,3,4 as bridges corretas

```
# Bridge 40
> interface bridge port add bridge=bridge40 interface=ether1 (terminal 3)
> interface bridge port add bridge=bridge40 interface=ether2 (terminal 4)
# Bridge 41
> interface bridge port add bridge=bridge41 interface=ether3 (terminal 2)
> interface bridge port add bridge=bridge41 interface=ether4 (terminal 4)
> interface bridge port add bridge=bridge41 interface=ether5 (router)

```

# Router

```
# configuração IP externo
> ip address add address=172.16.1.49/24 interface=ether1
# configuração IP local
> ip address add address=172.16.41.254/24 interface=ether2
# imprimir ip's
> ip address print

# Definir default gateway
> ip route add dst-address=0.0.0.0/0 gateway=172.16.1.254
# Definir route local
> ip route add dst-address=172.16.40.0/24 gateway=172.16.41.253
#
> ip route print
```