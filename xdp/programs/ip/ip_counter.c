#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>

struct packet_info {
    __be32 source_ip;
    __be32 destination_ip;
    __u8 protocol;
    __u8 ttl;
    __be16 h_proto;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4096);
    __type(key, struct packet_info);
    __type(value, __u64);
} ether_ip_map SEC(".maps");

SEC("prog")
int xdp_ip_counter(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;

    if ((void *)(eth + 1) > data_end)
        return XDP_DROP;

    struct iphdr *iph = (struct iphdr *)(eth + 1);

    if ((void *)(iph + 1) > data_end)
        return XDP_DROP;

    struct packet_info info;
    info.source_ip = iph->saddr;
    info.destination_ip = iph->daddr;
    info.protocol = iph->protocol;
    info.ttl = iph->ttl;
    info.h_proto = eth->h_proto;

    __u64 *existing_value = bpf_map_lookup_elem(&ether_ip_map, &info);
    __u64 value;

    if (existing_value) {
        value = *existing_value + 1;
    } else {
        value = 1; 
    }

    bpf_map_update_elem(&ether_ip_map, &info, &value, BPF_ANY);

    return XDP_PASS;
}
