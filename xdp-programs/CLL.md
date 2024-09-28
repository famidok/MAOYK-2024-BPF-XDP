# Compile & Load & Look (CLL)

## Requirements

The following tools need to be installed:

### Debian/Ubuntu

```bash
sudo apt install clang llvm gcc libbpf-dev libxdp-dev xdp-tools bpftool linux-headers-$(uname -r)
```

### Fedora

```bash
sudo dnf install clang llvm gcc libbpf libbpf-devel libxdp libxdp-devel xdp-tools bpftool kernel-headers
```

### Arch Linux

```bash
sudo pacman -S clang llvm gcc libbpf libxdp xdp-tools bpftool linux-headers
```

For installation with **yay**:

```bash
yay -S clang llvm gcc libbpf libxdp xdp-tools bpftool linux-headers
```

## Compilation

To compile the source code, use the following command:

```bash
clang -O2 -g -Wall -target bpf -c SOURCE_CODE -o OBJECT_FILE
```

Replace ```SOURCE_CODE``` with the name of the ```.c``` source file you want to compile. The generated ```OBJECT_FILE``` will be the ```.o``` object file.

## Loading

### Loading with IP_LINK

To load the XDP Program onto the ```INTERFACE_NAME``` interface:

**LOAD:**
```bash
sudo ip link set INTERFACE_NAME xdpgeneric obj OBJECT_FILE sec SECTION_NAME
```

**UNLOAD:**
```bash
sudo ip link set INTERFACE_NAME xdpgeneric off
```

### Loading with SKB

To load onto the ```INTERFACE_NAME``` interface in SKB mode:

**LOAD:**
```bash
sudo xdp-loader load -m skb -s SECTION_NAME INTERFACE_NAME OBJECT_FILE
```

**UNLOAD:**
```bash
sudo xdp-loader unload -a INTERFACE_NAME
```
# MAP Operations

## How to pin MAP

```bash
sudo bpftool map pin name MAP_NAME
```

## How to dump MAP

```bash
sudo bpftool map dump name MAP_NAME
```
