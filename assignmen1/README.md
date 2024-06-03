Compiling and Running
Compile the eBPF program:

Save the eBPF program to a file named xdp_drop.c and compile it using clang:
```bash
clang -O2 -target bpf -c xdp_drop.c -o xdp_drop.o
```
Compile the user-space program:

Save the user-space program to a file named user_prog.c and compile it:
```bash
gcc -o user_prog user_prog.c -lbpf
```
Load the eBPF program:

Use the ip command to attach the eBPF program to a network interface (replace eth0 with your actual interface name):
```bash
ip link set dev eth0 xdp obj xdp_drop.o sec xdp
```
Create and mount the BPF filesystem:
```bash
mount -t bpf bpf /sys/fs/bpf/
```
Pin the map:
```bash
bpftool map pin id <map_id> /sys/fs/bpf/drop_port_map
```
Replace <map_id> with the actual ID of the map, which can be found using bpftool map show.

Run the user-space program to set the port:

```bash
./user_prog 4040
```
You can change the port number by running the program again with a different port.

