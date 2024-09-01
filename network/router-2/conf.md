# Network configuration for Router-2

**Note:** Router-2 runs Raspbian OS Lite on a Raspberry Pi 4B.

## Enable IP forwarding

To forward packets destined for addresses other than the router's own, you need to enable the `net.ipv4.ip_forward` kernel parameter.

**1.Check Current IP Forwarding Status**

`sysctl net.ipv4.ip_forward`
