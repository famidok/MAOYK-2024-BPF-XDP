# Network configuration for Router-2

**Note:** Router-2 runs Raspbian OS Lite on a Raspberry Pi 4B.

## Configuring VLAN on eth0

To set up VLAN (Virtual LAN) on your network interface eth0, you need to configure both the base interface and the VLAN interface. This configuration ensures that your router can handle traffic on the specified VLAN.

### Configuration Steps

**1. Edit the Network Interfaces Configuration**

You need to update the network interfaces configuration file to include settings for both eth0 and eth0.202. This configuration will define how the base interface and the VLAN interface are set up.

Create and open the network interfaces configuration file:

```bash
sudo touch /etc/network/interfaces.d/eth0 && sudo nano /etc/network/interfaces.d/eth0
```

**2. Add Configuration for Base Interface `eth0`**

Add the following configuration to specify that `eth0` should be managed manually:

```bash
auto eth0
iface eth0 inet manual
```
**Explanation:**

- `auto eth0`: Ensures that eth0 is automatically brought up at boot time.
- `iface eth0 inet manual`: Configures eth0 with a manual IP configuration, meaning it does not use DHCP or static IP directly.
    
    
**3. Add Configuration for VLAN Interface `eth0.202`**

Add the following configuration to define the VLAN interface `eth0.202`:

```bash
auto eth0.202
iface eth0.202 inet static
address 10.10.202.1
netmask 255.255.255.0
vlan-raw-device eth0
```

**Explanation:**
- `auto eth0.202`: Ensures that the VLAN interface eth0.202 is automatically brought up at boot time.
- `iface eth0.202 inet static`: Configures eth0.202 with a static IP address.
- `address 10.10.202.1`: Sets the IP address for the VLAN interface.
- `netmask 255.255.255.0`: Defines the subnet mask for the VLAN interface.
- `vlan-raw-device eth0`: Specifies the base device (eth0) for the VLAN.
      
**4. Apply the Configuration**

After saving the configuration file, you need to bring up the interfaces. You can do this by restarting the networking service or rebooting the system:

```bash
sudo ifdown eth0 && sudo ifup eth0
sudo ifdown eth0.202 && sudo ifup eth0.202
```

Alternatively, reboot the system to apply the changes:

```bash
sudo reboot
```

### Verifying the Configuration

To verify that the interfaces are correctly configured, use the following commands:

```bash
ip addr show eth0
ip addr show eth0.202
```

These commands will display the IP addresses and configuration of the eth0 and eth0.202 interfaces.


## Enable IP forwarding

To forward packets destined for addresses other than the router's own, you need to enable the `net.ipv4.ip_forward` kernel parameter.

### Check Current IP Forwarding Status

To view the current IP forwarding status, run the following command:

```bash
sysctl net.ipv4.ip_forward
```

**Example Output:** 

`net.ipv4.ip_forward = 0`

- `0` value indicates that is disabled.
- `1` value indicates that is enabled.

### Enable IP Forwarding

To enable IP forwarding, follow these steps:

**1. Open the /etc/sysctl.conf file for editing:**

```bash
sudo nano /etc/sysctl.conf
```

**2. Locate the line that specifies `net.ipv4.ip_forward` and change it to:**

```bash
net.ipv4.ip_forward = 1
```
   
**4. Apply the changes with the following command:**

```bash 
sudo sysctl -p
```

## Add IP Route Configuration

To properly configure network routing on Router-2, you need to add the following IP route rules. These rules help the router correctly route packets between different network segments.

### Route Rules

**1. Default Route**

```bash
ip route add default via 10.10.202.2 dev eth0.202
```

**Explanation:** This rule sets the default gateway for all outbound traffic. Packets destined for addresses not specified in other routes will be forwarded to 10.10.202.2 via the eth0.202 interface. This is crucial for ensuring that the router can communicate with external networks or the internet.


**2. Specific Network Route**

```bash
ip route add 10.10.201.0/24 via 10.10.202.2 dev eth0.202
```

**Explanation:** This rule directs traffic destined for the 10.10.201.0/24 network to the gateway 10.10.202.2 through the eth0.202 interface. This ensures that packets intended for the 10.10.201.0/24 subnet are routed correctly through the appropriate gateway.
   

**3. Directly Connected Network**

```bash
ip route add 10.10.202.0/24 dev eth0.202 proto kernel scope link src 10.10.202.1
```
   
**Explanation:** This rule specifies that the 10.10.202.0/24 network is directly connected to the eth0.202 interface. The proto kernel scope link part indicates that this route is directly managed by the kernel, and src 10.10.202.1 specifies the source IP address used for this network. This is necessary for the router to correctly handle traffic within the 10.10.202.0/24 subnet.

### Applying the Configuration

```bash
sudo ip route add default via 10.10.202.2 dev eth0.202
sudo ip route add 10.10.201.0/24 via 10.10.202.2 dev eth0.202
sudo ip route add 10.10.202.0/24 dev eth0.202 proto kernel scope link src 10.10.202.1
```

### Verifying the Routes

To verify that the routes have been correctly added, use the following command:

```bash
ip route
```

This command will display the current routing table and allow you to check that the routes are correctly configured.






