# Compile Both Programs
```bash
→ make
clang++ -o main.clang main.cpp -g -ldw
g++ -o main.gcc main.cpp -O0 -g -ldw
main.cpp:74:37: warning: ‘always_inline’ function might not be inlinable [-Wattributes]
   74 | __attribute__((always_inline)) void terminate_with_stacktrace()
      |                                     ^~~~~~~~~~~~~~~~~~~~~~~~~
```

# Output with GCC

```bash
→ ./main.gcc
Stacktrace of 6 frames:
0: 0x5633afcf2543 hodormania() at /home/jayesh/xlab/backtrace/main.cpp:77
1: 0x5633afcf2682 godormania() at /home/jayesh/xlab/backtrace/main.cpp:97
2: 0x5633afcf268e fodormania() at /home/jayesh/xlab/backtrace/main.cpp:102
3: 0x5633afcf269a main at /home/jayesh/xlab/backtrace/main.cpp:107
4: 0x7f3ce9db8152 __libc_start_main
5: 0x5633afcf223e _start
```

# Output with Clang

```bash
→ ./main.clang
Stacktrace of 6 frames:
0: 0x5649f82c95ec hodormania()
1: 0x5649f82c9799 godormania()
2: 0x5649f82c97a9 fodormania()
3: 0x5649f82c97b9 main
4: 0x7f9536474152 __libc_start_main
5: 0x5649f82c929e _start
```
