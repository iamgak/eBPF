#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/ip.h>
#include <linux/tcp.h>

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);   // PID
    __type(value, char[16]);  // Process name
    __uint(max_entries, 1024);
} process_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 1);
    __type(key, u32);
    __type(value, u32);
} allowed_port_map SEC(".maps");

SEC("cgroup_skb/egress")
int allow_port_for_process(struct __sk_buff *skb) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    u32 key = 0;
    u32 *allowed_port;
    char *process_name;
    struct ethhdr *eth = (void *)(long)skb->data;
    struct iphdr *ip;
    struct tcphdr *tcp;

    if ((void *)eth + sizeof(*eth) > (void *)(long)skb->data_end)
        return 1; // Allow other packets to pass

    ip = (struct iphdr *)(eth + 1);
    if ((void *)ip + sizeof(*ip) > (void *)(long)skb->data_end)
        return 1;

    if (ip->protocol != IPPROTO_TCP)
        return 1;

    tcp = (struct tcphdr *)(ip + 1);
    if ((void *)tcp + sizeof(*tcp) > (void *)(long)skb->data_end)
        return 1;

    allowed_port = bpf_map_lookup_elem(&allowed_port_map, &key);
    process_name = bpf_map_lookup_elem(&process_map, &pid);

    if (allowed_port && process_name && tcp->dest == bpf_htons(*allowed_port)) {
        return 1; // Allow packet
    }

    return 0; // Drop packet
}

char _license[] SEC("license") = "GPL";
