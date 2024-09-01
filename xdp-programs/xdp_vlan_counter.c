#include <linux/bpf.h>
#include <linux/if_vlan.h>
#include <linux/if_ether.h>
#include <bpf/bpf_helpers.h>
#include <arpa/inet.h>

#define VLAN_VID_MASK 0x0FFF

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 4096);
	__type(key, __u16);
	__type(value, __u64);
} vlan_counter_map SEC(".maps");

struct vlan_ethhdr {
	unsigned char h_dest[ETH_ALEN];
	unsigned char h_source[ETH_ALEN];
	__be16 h_vlan_proto;
	__be16 h_vlan_TCI;
	__be16 h_vlan_encapsulated_proto;
};

struct vlan_hdr {
	__be16 h_vlan_TCI;
	__be16 h_vlan_encapsulated_proto;
};
SEC("prog")
int xdp_vlan_counter(struct xdp_md *ctx) {
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;

	struct ethhdr *eth = data;

	if ((void *)(eth + 1) > data_end)
		return XDP_DROP;

	if (eth->h_proto == htons(ETH_P_8021Q)) {
		struct vlan_hdr *vhdr = (void *)(eth + 1);

		if ((void *)(vhdr + 1) > data_end)
			return XDP_DROP;

		__u16 vlan_id = htons(vhdr->h_vlan_TCI) & VLAN_VID_MASK;

		__u64 value = 0;
		__u64 *existing_value = bpf_map_lookup_elem(&vlan_counter_map, &vlan_id);

		if (existing_value) {
			value = *existing_value + 1;
		}

		bpf_map_update_elem(&vlan_counter_map, &vlan_id, &value, BPF_ANY);
	}

	return XDP_PASS;
}
