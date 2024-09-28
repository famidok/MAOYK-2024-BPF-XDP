#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <bpf/bpf_helpers.h>

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 4096);
	__type(key, __be32);
	__type(value, __u64);
} ether_ip_map SEC(".maps");

struct iphdr {
	__u8 ihl: 4;
	__u8 version: 4;
	__u8 tos;
	__be16 tot_len;
	__be16 id;
	__be16 frag_off;
	__u8 ttl;
	__u8 protocol;
	__sum16 check;
	union {
		struct {
			__be32 saddr;
			__be32 daddr;
		};
		struct {
			__be32 saddr;
			__be32 daddr;
		} addrs;
	};
};

SEC("prog")
int xdp_ether_type_counter(struct xdp_md *ctx) {
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;

	struct ethhdr *eth = data;

	if ((void *)(eth + 1) > data_end)
		return XDP_DROP;

	struct iphdr *iph = (struct iphdr *)(eth + 1);

	if ((void *)(iph + 1) > data_end)
		return XDP_DROP;

	__be32 dst_ip = iph->daddr;

	__u64 value = 0;

	__u64 *existing_value = bpf_map_lookup_elem(&ether_ip_map, &dst_ip);
	if (existing_value) {
		value = *existing_value + 1;
	} else {
		value = 1;
	}
	bpf_map_update_elem(&ether_ip_map, &dst_ip, &value, BPF_ANY);

	return XDP_PASS;
}
