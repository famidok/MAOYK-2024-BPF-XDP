#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/if_vlan.h>
#include <bpf/bpf_helpers.h>
#include <arpa/inet.h>

#define TARGET_IP 0x0A0AC964

struct vlan_hdr {
    __be16 h_vlan_TCI;
    __be16 h_vlan_encapsulated_proto;
};

SEC("prog")
int xdp_ip_ro(struct xdp_md *ctx) {
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;

    struct ethhdr *eth = data;

    if ((void *)(eth + 1) > data_end) {
        return XDP_DROP;
    }

    if (eth->h_proto == htons(ETH_P_8021Q)) {
        struct vlan_hdr *vhdr = (void *)(eth + 1);

        if ((void *)(vhdr + 1) > data_end)
            return XDP_DROP;

        struct iphdr *iph = (struct iphdr *)(vhdr + 1);

        if ((void *)(iph + 1) > data_end)
            return XDP_DROP;

        if (iph->protocol == IPPROTO_ICMP) {
            __be32 old_ip = iph->daddr;
            
            iph->daddr = htonl(TARGET_IP);
            
            iph->check = 0;
            iph->check = bpf_csum_diff(&old_ip, sizeof(old_ip), &iph->daddr, sizeof(iph->daddr), 0);
        }
    }

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
