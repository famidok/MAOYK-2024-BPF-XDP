#include <stdio.h>
#include <arpa/inet.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

#define MAP_PATH "/sys/fs/bpf/ip_map"

int main() {
    int map_fd = bpf_obj_get(MAP_PATH);
    if (map_fd < 0) {
        perror("No mapperino pepperino!");
        return 1;
    }

    __be32 key, next_key;
    __u64 value;
    struct in_addr ip_addr;

    key = 0;
    while (bpf_map_get_next_key(map_fd, &key, &next_key) == 0) {
        if (bpf_map_lookup_elem(map_fd, &next_key, &value) == 0) {
            ip_addr.s_addr = next_key;  

            printf("Source IP: %s - Packet Count: %lu\n", inet_ntoa(ip_addr), value);
        }
        key = next_key;
    }

    return 0;
}
