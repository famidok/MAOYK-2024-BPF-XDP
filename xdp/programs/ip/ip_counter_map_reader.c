#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <bpf/bpf.h>

#define MAP_PATH "/sys/fs/bpf/xdp/packet_info_map"

void get_protocol_name(__be16 h_proto, char *protocol_name) {
    switch (ntohs(h_proto)) {
        case ETH_P_IP:
            strcpy(protocol_name, "IPv4");
            break;
        case ETH_P_IPV6:
            strcpy(protocol_name, "IPv6");
            break;
        case ETH_P_ARP:
            strcpy(protocol_name, "ARP");
            break;
        case ETH_P_RARP:
            strcpy(protocol_name, "Reverse ARP");
            break;
        case ETH_P_8021Q:
            strcpy(protocol_name, "802.1Q VLAN");
            break;
        case ETH_P_8021AD:
            strcpy(protocol_name, "802.1ad VLAN");
            break;
        case ETH_P_PPP_DISC:
            strcpy(protocol_name, "PPPoE Discovery");
            break;
        case ETH_P_PPP_SES:
            strcpy(protocol_name, "PPPoE Session");
            break;
        case ETH_P_MPLS_UC:
            strcpy(protocol_name, "MPLS Unicast");
            break;
        case ETH_P_MPLS_MC:
            strcpy(protocol_name, "MPLS Multicast");
            break;
        case ETH_P_BATMAN:
            strcpy(protocol_name, "B.A.T.M.A.N.");
            break;
        case ETH_P_LLDP:
            strcpy(protocol_name, "LLDP");
            break;
        case ETH_P_802_3:
            strcpy(protocol_name, "Ethernet");
            break;
        case ETH_P_LOOPBACK:
            strcpy(protocol_name, "Ethernet Loopback");
            break;
        case ETH_P_1588:
            strcpy(protocol_name, "PTP (IEEE 1588)");
            break;
        case ETH_P_FCOE:
            strcpy(protocol_name, "Fibre Channel over Ethernet");
            break;
        case ETH_P_TEB:
            strcpy(protocol_name, "Transparent Ethernet Bridging");
            break;
        case ETH_P_AOE:
            strcpy(protocol_name, "ATA over Ethernet");
            break;
        case ETH_P_TIPC:
            strcpy(protocol_name, "TIPC");
            break;
        case ETH_P_EDSA:
            strcpy(protocol_name, "EDSA (Ethernet Data Separator)");
            break;
        case ETH_P_AF_IUCV:
            strcpy(protocol_name, "IBM IUCV");
            break;
        case ETH_P_802_2:
            strcpy(protocol_name, "LLC");
            break;
        case ETH_P_IPX:
            strcpy(protocol_name, "IPX");
            break;
        case ETH_P_ATALK:
            strcpy(protocol_name, "AppleTalk");
            break;
        case ETH_P_AARP:
            strcpy(protocol_name, "AppleTalk ARP");
            break;
        case ETH_P_8021AH:
            strcpy(protocol_name, "802.1ah Backbone VLAN");
            break;
        case ETH_P_802_EX1:
            strcpy(protocol_name, "802.1AE MACsec");
            break;
        case ETH_P_DEC:
            strcpy(protocol_name, "DECnet Phase IV");
            break;
        case ETH_P_DNA_DL:
            strcpy(protocol_name, "DEC DNA Dump/Load");
            break;
        case ETH_P_DNA_RC:
            strcpy(protocol_name, "DEC DNA Remote Console");
            break;
        case ETH_P_DNA_RT:
            strcpy(protocol_name, "DEC DNA Routing");
            break;
        case ETH_P_LAT:
            strcpy(protocol_name, "DEC Local Area Transport");
            break;
        case ETH_P_DIAG:
            strcpy(protocol_name, "DEC Diagnostics");
            break;
        case ETH_P_CUST:
            strcpy(protocol_name, "DEC Customer Use");
            break;
        case ETH_P_SCA:
            strcpy(protocol_name, "DEC Systems Communication Architecture");
            break;
        case ETH_P_LINK_CTL:
            strcpy(protocol_name, "Ethernet Link Control");
            break;
        case ETH_P_ATMFATE:
            strcpy(protocol_name, "Frame Relay");
            break;
        case ETH_P_X25:
            strcpy(protocol_name, "X.25 Layer 2");
            break;
        case ETH_P_TR_802_2:
            strcpy(protocol_name, "802.2 LLC on Token Ring");
            break;
        case ETH_P_SNAP:
            strcpy(protocol_name, "SNAP");
            break;
        case ETH_P_WAN_PPP:
            strcpy(protocol_name, "WAN PPP");
            break;
        case ETH_P_PPP_MP:
            strcpy(protocol_name, "PPP Multilink");
            break;
        case ETH_P_LOCALTALK:
            strcpy(protocol_name, "LocalTalk");
            break;
        case ETH_P_CAN:
            strcpy(protocol_name, "CAN Protocol");
            break;
        case ETH_P_CANFD:
            strcpy(protocol_name, "CAN FD Protocol");
            break;
        case ETH_P_PPPTALK:
            strcpy(protocol_name, "PPPTalk");
            break;
        default:
            strcpy(protocol_name, "Unknown Protocol");
            break;
    }
}


void get_ip_protocol_name(__u8 protocol, char *ip_protocol_name) {
    switch (protocol) {
        case IPPROTO_ICMP:
            strcpy(ip_protocol_name, "ICMP");
            break;
        case IPPROTO_TCP:
            strcpy(ip_protocol_name, "TCP");
            break;
        case IPPROTO_UDP:
            strcpy(ip_protocol_name, "UDP");
            break;
        case IPPROTO_IGMP:
            strcpy(ip_protocol_name, "IGMP");
            break;
        case IPPROTO_GRE:
            strcpy(ip_protocol_name, "GRE");
            break;
        case IPPROTO_AH:
            strcpy(ip_protocol_name, "AH (Authentication Header)");
            break;
        case IPPROTO_ESP:
            strcpy(ip_protocol_name, "ESP (Encapsulating Security Payload)");
            break;
        case IPPROTO_SCTP:
            strcpy(ip_protocol_name, "SCTP");
            break;
        case IPPROTO_UDPLITE:
            strcpy(ip_protocol_name, "UDP Lite");
            break;
        case IPPROTO_PIM:
            strcpy(ip_protocol_name, "PIM (Protocol Independent Multicast)");
            break;
        case IPPROTO_EGP:
            strcpy(ip_protocol_name, "EGP (Exterior Gateway Protocol)");
            break;
        case IPPROTO_IPIP:
            strcpy(ip_protocol_name, "IPIP (IPIP tunnels (older KA9Q tunnels use 94))");
            break;
        case IPPROTO_PUP:
            strcpy(ip_protocol_name, "PUP Protocol");
            break;
        case IPPROTO_RAW:
            strcpy(ip_protocol_name, "RAW (Raw IP packets)");
            break;
        case IPPROTO_DCCP:
            strcpy(ip_protocol_name, "DCCP (Datagram Congestion Control Protocol)");
            break;
        case IPPROTO_IPV6:
            strcpy(ip_protocol_name, "IPv6");
            break;
        case IPPROTO_RSVP:
            strcpy(ip_protocol_name, "RSVP (Resource ReSerVation Protocol)");
            break;
        case IPPROTO_MPLS:
            strcpy(ip_protocol_name, "MPLS");
            break;
        case IPPROTO_ETHERNET:
            strcpy(ip_protocol_name, "Ethernet encapsulation");
            break;
        case IPPROTO_ICMPV6:
            strcpy(ip_protocol_name, "ICMPv6");
            break;
        default:
            strcpy(ip_protocol_name, "Unknown Protocol");
            break;
    }
}


struct packet_info {
    __be32 source_ip;
    __be32 destination_ip;
    __u8 protocol;
    __u8 ttl;
    __be16 h_proto;
};

int main(void) {

    char protocol_name[32];
    char ip_protocol_name[36];

    int map_fd = bpf_obj_get(MAP_PATH);

    if (map_fd < 0) {
        perror("No mapperino pepperino!");
        return 1;
    }

    struct packet_info key, next_key;
    __u64 value;
    memset(&key, 0, sizeof(key));

    printf("+----------------------+----------------------+---------------------+-----+----------+-----------------------+\n");
    printf("| Source IP            | Destination IP       | IP Protocol Type    | TTL | Count    | EtherType             |\n");
    printf("+----------------------+----------------------+---------------------+-----+----------+-----------------------+\n");

    
    while (1) { 
        memset(&key, 0, sizeof(key)); 
        while (bpf_map_get_next_key(map_fd, &key, &next_key) == 0) {
            if (bpf_map_lookup_elem(map_fd, &next_key, &value) == 0) {
                char source_ip_str[INET_ADDRSTRLEN];
                char destination_ip_str[INET_ADDRSTRLEN];

                get_protocol_name(next_key.h_proto, protocol_name);
                get_ip_protocol_name(next_key.protocol, ip_protocol_name);
                inet_ntop(AF_INET, &next_key.source_ip, source_ip_str, INET_ADDRSTRLEN);
                inet_ntop(AF_INET, &next_key.destination_ip, destination_ip_str, INET_ADDRSTRLEN);
                printf("| %-20s | %-20s | %-19s | %-3u | %-8llu | %-21s |\n", source_ip_str, destination_ip_str, ip_protocol_name, next_key.ttl, value, protocol_name);
            } else {
                perror("Failed to lookup value.");
            }

            key = next_key; 
        }

        printf("+----------------------+----------------------+---------------------+-----+----------+-----------------------+\n");
        usleep(500000); 
    }

    return 0;
}

