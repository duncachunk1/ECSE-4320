# Parallel Compression - Project 1

The objective of this assignment was to utilize multithreaded programming in order to efficiently compress large chunks of
data. Compression was accomplished through Facebook's open sourced ZSTD library.

## Installation and Environment

### ZSTD Library
In your terminal send the following command:
```
sudo apt-get install libzstd-dev
```

## Compilation

### Build

Make sure that `common.h` is in the same directory as `zstd_compression.c`

The program can be compiled using `gcc` or `g++`
```
g++ -g zstd_compression.c -o a.out -Wall -Wextra -pthread -lzstd
```
```
gcc -o a.out zstd_compression.c -lzstd -pthread
```

### Execution

```
./a.out <num_of_threads> <compression_size> <file name>
```

## About the Code
This prject utilized the `streaming_compression_thread_pool.c` file from the example folder in ZSTD to support this program. The `common.h` file serves to make sure that the program does not inappropriately execute any of the functions in the setup process.

## Testing and Analysis

Testing consisted of the efficiency associated with the number of threads as well as the amount of data that needs to be
compressed.

| Filename	| Old Size(KB)	|New Size (KB)	| # Threads	| Compression Time (s)|
|---------------|---------------|---------------|---------------|-----------------------|
| wot.txt	| 24400         |	8900	| 4          	| 0.26367	        |
| 		|	        |		| 8          	| 0.25668	        |
| 		|	        |		| 12          	| 0.25221	        |
| 		|	        |		| 16          	| 0.25556	        |
| 		|	        |		| 20          	| 0.25016	        |
| LI_short.txt	| 103228        |	89	| 4          	| 0.06994	        |
| 		|	        |		| 8          	| 0.07415	        |
| 		|	        |		| 12          	| 0.06342	        |
| 		|	        |		| 16          	| 0.06187	        |
| 		|	        |		| 20          	| 0.06369	        |
| LI_long.txt	| 619366        |	331	| 4          	| 0.32485	        |
| 		|	        |		| 8          	| 0.33599	        |
| 		|	        |		| 12          	| 0.35398	        |
| 		|	        |		| 16          	| 0.37728	        |
| 		|	        |		| 20          	| 0.33719	        |


## Conclusion

While our compression proved to work successfully, increasing the thread count did not effect the speed at which the code executed compression. This is believed to be a result of operating and testing on a machine with only a single core CPU. The input data stream experiences a bottleneck effect as the input stream of data cannot compete with the speed of compression as we increase the thread count. In the future, this code should be put to the test on a computer with more than one CPU core, and with larger sizes of input data.
