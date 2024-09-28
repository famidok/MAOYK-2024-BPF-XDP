#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <bpf/bpf_helpers.h>
#include <arpa/inet.h>

#define TARGET_IP 168479233
#define DROP_RATE 10


struct vlan_hdr {
	__be16 h_vlan_TCI;
	__be16 h_vlan_encapsulated_proto;
};

SEC("prog")
int xdp_troll(struct xdp_md *ctx) {
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;

	struct ethhdr *eth = data;
        struct vlan_hdr *vhdr = (void *)(eth + 1);

	if ((void *)(eth + 1) > data_end)
		return XDP_DROP;

       if (eth->h_proto == htons(ETH_P_8021Q)) {
       
	if ((void *)(vhdr + 1) > data_end)
       		return XDP_DROP;
	}
		
	struct iphdr *iph = (struct iphdr *)(vhdr + 1);

	if((void *)(iph + 1) > data_end)
		return XDP_DROP;

	if(iph->daddr != TARGET_IP) {
        	return XDP_DROP;
    	}


    	return XDP_PASS;



}
char _license[] SEC("license") = "GPL";
