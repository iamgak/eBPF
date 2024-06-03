## eBPF Port and Process-based Traffic Filtering
This project provides an eBPF program to filter network traffic based on both TCP port and process name. Only traffic on a specified TCP port (default 4040) is allowed for a given process name, while all other traffic for that process is dropped.

# Project Structure
- allow_port_for_process.c: The eBPF program that performs the traffic filtering.
- user_prog.c: The user-space program to configure the allowed port and process name.
# Prerequisites
- Linux kernel with eBPF support (version 4.18 or later recommended).
- clang and llvm for compiling the eBPF program.
- libbpf and bpftool for interacting with eBPF from user-space.
# Setup and Compilation
- Step 1: Compile the eBPF Program
Save the eBPF program to a file named allow_port_for_process.c and compile it using clang:
```bash
clang -O2 -target bpf -c allow_port_for_process.c -o allow_port_for_process.o
```
- Step 2: Compile the User-Space Program
Save the user-space program to a file named user_prog.c and compile it:
```bash
gcc -o user_prog user_prog.c -lbpf
```
- Step 3: Load the eBPF Program
Attach the eBPF program to a cgroup (replace /sys/fs/cgroup/mycgroup with your actual cgroup path):
```bash
bpftool prog load allow_port_for_process.o /sys/fs/bpf/allow_port_for_process
bpftool cgroup attach /sys/fs/cgroup/mycgroup egress pinned /sys/fs/bpf/allow_port_for_process
```
- Step 4: Create and Mount the BPF Filesystem
Mount the BPF filesystem if not already mounted:
```bash
mount -t bpf bpf /sys/fs/bpf/
```
- Step 5: Pin the Maps
Pin the maps to the BPF filesystem:
```bash
bpftool map pin id <allowed_port_map_id> /sys/fs/bpf/allowed_port_map
bpftool map pin id <process_map_id> /sys/fs/bpf/process_map
```
Replace <allowed_port_map_id> and <process_map_id> with the actual IDs of the maps, which can be found using bpftool map show.

- Step 6: Run the User-Space Program
Run the user-space program to set the allowed port and process name:
```bash
./user_prog 4040 myprocess
```
# Usage
- Changing the Allowed Port and Process Name
To change the allowed port and process name, simply run the user-space program with the new values:
```bash
./user_prog <port> <process_name>
```
For example, to allow traffic only on port 8080 for a process named "exampleprocess":
```bash
./user_prog 8080 exampleprocess
```
- Stopping the Program
To stop the eBPF program and remove the filters, you can detach the program from the cgroup:
```bash
bpftool cgroup detach /sys/fs/cgroup/mycgroup egress pinned /sys/fs/bpf/allow_port_for_process
```
Notes
Ensure that the BPF filesystem is mounted and accessible at /sys/fs/bpf.
Ensure that you have the necessary permissions to load eBPF programs and access the cgroup.
The user-space program currently uses the current process ID for demonstration. Modify it as necessary for your use case.

