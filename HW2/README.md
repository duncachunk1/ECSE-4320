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
./fp.out <matrix_size>
```

## About the Code

This project uses SIMD commands from the AVX family x86 instruction set to directly read and write from registers in memory to take advantage of memory locality.

The commands used for floating point matrix multiplication were:
- _mm256_loadu_ps
- _mm256_storeu_ps
- _mm256_fmadd_ps

## Code Structure

In our code two square matricies are multiplied together using both an intellegent and naive approach, we then compared the two together to measure the efficeincy gain
from the use of memory locality. 

The intellegent approach calculated the values of each matrix cell utilizing 1D vector to represent the 2D matricies we multiplied. First these vectors were split up into
smaller tiles of 8x8 and then iterated through using one set of for loops to iterate through the matrix as a whole and another set of for loops to iterate through the 
tiles internally. For each tile on the larger matricies we iterated through all the other tiles loading their rows and columns into 8 value vectors and multiply and
accumulated to the cells in the overall matrix that they contributed to.

The naive approach simply interated through every cell in the matrix and multiplied and added the full row and column from the input matricies all at once instead of
taking advantage of memory locality in the cache.

## Testing and Analysis

Testing consisted of the comparison between the intellegent and naive approaches

### Floating Point Matricies
| Matrix Size	| Time for Intellegent (s)	| Time for Naive (s)	| % Efficiency |
|---------------|---------------|---------------|------------|
| 100x100	| 0.000318 |	0.001799	| 15.02 |
| 1000x1000		|	0.098645 |	0.943665	| 9.46407 |
| 10000x10000		|	302.385559 |		| 



## Conclusion

While our compression proved to work successfully, increasing the thread count did not effect the speed at which the code executed compression. This is believed to be a result of operating and testing on a machine with only a single core CPU. The input data stream experiences a bottleneck effect as the input stream of data cannot compete with the speed of compression as we increase the thread count. In the future, this code should be put to the test on a computer with more than one CPU core, and with larger sizes of input data.
