#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DROP_PORT_MAP_PATH "/sys/fs/bpf/drop_port_map"

void usage(char *prog) {
    fprintf(stderr, "Usage: %s <port>\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage(argv[0]);
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        exit(EXIT_FAILURE);
    }

    int map_fd = bpf_obj_get(DROP_PORT_MAP_PATH);
    if (map_fd < 0) {
        perror("bpf_obj_get");
        exit(EXIT_FAILURE);
    }

    u32 key = 0;
    u32 value = port;

    if (bpf_map_update_elem(map_fd, &key, &value, BPF_ANY) < 0) {
        perror("bpf_map_update_elem");
        exit(EXIT_FAILURE);
    }

    printf("Updated drop port to %d\n", port);
    close(map_fd);
    return 0;
}
