#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALLOWED_PORT_MAP_PATH "/sys/fs/bpf/allowed_port_map"
#define PROCESS_MAP_PATH "/sys/fs/bpf/process_map"

void usage(char *prog) {
    fprintf(stderr, "Usage: %s <port> <process_name>\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        usage(argv[0]);
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        exit(EXIT_FAILURE);
    }

    char *process_name = argv[2];
    if (strlen(process_name) > 15) {
        fprintf(stderr, "Process name too long.\n");
        exit(EXIT_FAILURE);
    }

    int port_map_fd = bpf_obj_get(ALLOWED_PORT_MAP_PATH);
    if (port_map_fd < 0) {
        perror("bpf_obj_get");
        exit(EXIT_FAILURE);
    }

    int process_map_fd = bpf_obj_get(PROCESS_MAP_PATH);
    if (process_map_fd < 0) {
        perror("bpf_obj_get");
        exit(EXIT_FAILURE);
    }

    u32 key = 0;
    u32 value = port;
    if (bpf_map_update_elem(port_map_fd, &key, &value, BPF_ANY) < 0) {
        perror("bpf_map_update_elem");
        exit(EXIT_FAILURE);
    }

    printf("Updated allowed port to %d\n", port);

    u32 pid = getpid(); // For demonstration, we use the current process ID
    if (bpf_map_update_elem(process_map_fd, &pid, process_name, BPF_ANY) < 0) {
        perror("bpf_map_update_elem");
        exit(EXIT_FAILURE);
    }

    printf("Added process %s with PID %d to the process map\n", process_name, pid);

    close(port_map_fd);
    close(process_map_fd);
    return 0;
}
