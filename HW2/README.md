# Matrix Multiplication - Project 2

The objective of this assignment was to utilize the cpu cache loading scheme to preform matrix multiplication minimizing cache misses.

## Installation and Environment

To Install please make sure that you have downloaded the full folder

System specs:
8 core 11 gen intel core i7 @ 2.3GHz with 32 GB of ram
Ubuntu 22.04.1 LTS

## Compilation

For the floating point programm please use

```
 gcc -mavx -march=native -mfpmath=both -O3 -o fp.out floating_point_tiling.c
```

## Execution

Once you have compiled you may run with the command
```
./fp.out <matrix_size> f
```

## About the Code

This project uses SIMD commands from the AVX family x86 instruction set to directly read and write from registers in memory to take advantage of memory locality.

The commands used for floating point matrix multiplication were:
- _mm256_loadu_ps
- _mm256_storeu_ps
- _mm256_fmadd_ps

## Code Structure

In our code the matricies are multiplied together using both an Intellegent and Naive approach, we then compared the two together to measure the efficeincy gain
from the use of memory locality. 

The Intellegent approach calculated the values of each matrix cell utilizing 1D vector to represent the 2D matricies we multiplied. First these vectors were split up into
smaller tiles of 8x8 and then iterated through using one set of for loops to iterate through the matrix as a whole and another set of for loops to iterate through the 
tiles internally. For each tile on the larger matricies we iterated through all the other tiles loading their rows and columns into 8 value vectors and multiply and
accumulated to the cells in the overall matrix that they contributed to.

## Testing and Analysis

Testing consisted of the comparason be

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
