# Explanation of Construct

```bash
make(chan func(), 10):
```
- This creates a buffered channel cnp that can hold up to 10 values, where each value is a function (func()).
- Use-case: Buffered channel will asynchronously allow max 10 (func()) to execute without blocking.

``` bash 
for i := 0; i < 4; i++ { ... }:
```
- This loop runs 4 iterations, creating 4 new goroutines.
- Use-case: The loop is used to create multiple new goroutines that can handle tasks concurrently.

``` bash 
go func() { ... }():
```
- Create a new goroutine.
- Use-case: Goroutines are lightweight threads managed by the Go runtime, used for concurrent execution of tasks.

``` bash 
for f := range cnp { ... }:
```

- This is a range loop over the channel cnp. It continuously receives functions from the channel and executes them by calling f().
- Use-case: The range loop on a channel is used to process incoming data (in this case, functions) until the channel is closed.

``` bash 
cnp <- func() { ... }:
```
- This sends a function that prints "HERE1" to the channel cnp.
- Use-case: Sending functions through channels allows for dynamic execution of tasks in worker goroutines.

``` bash 
fmt.Println("HERE1"):
```
- This is the function that will be executed by the worker goroutine, printing "HERE1" to the standard output.
- It'll not execute because main goroutine finish executing and exit the program.
- time.Sleep(1 * time.Second) To ensure the main function does not exit before the goroutine has processed all functions. 


``` bash 
fmt.Println("Hello"):
```
- This prints "Hello" to the standard output from the main goroutine.