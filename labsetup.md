# Ligacoes fisicas

```
TUX42S0 -> Switch Porta Consola

TUX44E0  -> Switch Porta 1 
TUX43E0  -> Switch Porta 2
TUX42E0  -> Switch Porta 3
TUX44E1  -> Switch Porta 4
```
# Configurar ip's nos terminais localmente

## terminal 4 (172.16.40.254 -> 00:21:5a:5a:7b:ea)
```
> ifconfig eth0 up
> ifconfig eth0 172.16.40.254/24
> ifconfig eth1 172.16.41.253/24
```
## terminal 3 (172.16.40.1 -> 00:21:5a:61:2f:d4)
```
> ifconfig eth0 up
> ifconfig eth0 172.16.40.1/24
> route add -net 172.16.41.0/24 gw 172.16.40.254
```

## terminal 2 (172.16.41.1/24)
```
> ifconfig eth0 up
> ifconfig eth0 172.16.41.1/24
> route add -net 172.16.40.0/24 gw 172.16.41.253
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
```

## Adicionar tux 2,3,4 as bridges corretas

```
> interface bridge port add bridge=bridge40 interface=ether1 (terminal 3)
> interface bridge port add bridge=bridge40 interface=ether2 (terminal 4)

> interface bridge port add bridge=bridge41 interface=ether3 (terminal 2)
> interface bridge port add bridge=bridge41 interface=ether4 (terminal 4)
```