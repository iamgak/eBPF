#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/ip.h>
#include <linux/tcp.h>

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 1);
    __type(key, u32);
    __type(value, u32);
} drop_port_map SEC(".maps");

SEC("xdp")
int xdp_drop_tcp_port(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    struct iphdr *ip;
    struct tcphdr *tcp;
    u32 key = 0;
    u32 *port;

    if ((void *)eth + sizeof(*eth) > data_end)
        return XDP_PASS;

    ip = (struct iphdr *)(eth + 1);
    if ((void *)ip + sizeof(*ip) > data_end)
        return XDP_PASS;

    if (ip->protocol != IPPROTO_TCP)
        return XDP_PASS;

    tcp = (struct tcphdr *)(ip + 1);
    if ((void *)tcp + sizeof(*tcp) > data_end)
        return XDP_PASS;

    port = bpf_map_lookup_elem(&drop_port_map, &key);
    if (port && tcp->dest == bpf_htons(*port)) {
        return XDP_DROP;
    }

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
