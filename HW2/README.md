# Matrix Multiplication - Project 2

The objective of this assignment was to utilize the cpu cache loading scheme to preform matrix multiplication minimizing cache misses.

## Installation and Environment

To Install please make sure that you have downloaded the full folder

System specs:
8 core 11 gen intel core i7 @ 2.3GHz with 32 GB of ram
Ubuntu 22.04.1 LTS

## Compilation

Use the following command to compile the program

```
 gcc -mavx -march=native -mfpmath=both -O3 -o a.out floating_point_tiling.c
```

## Execution

Once you have compiled you may run with the command
```
./a.out <matrix_size> <'f' for floating point 's' for short>
```

## About the Code

This project uses SIMD commands from the AVX family x86 instruction set to directly read and write from registers in memory to take advantage of memory locality.

The commands used for floating point matrix multiplication were:
- _mm256_loadu_ps
- _mm256_storeu_ps
- _mm256_fmadd_ps
- _mm256_set_epi16
- _mm256_mulhi_epi16
- _mm256_mullo_epi16
- _mm256_add_epi32
- _mm256_extract_epi16

## Code Structure

In our code two square matricies are multiplied together using both an intellegent and naive approach, we then compared the two together to measure the efficeincy gain
from the use of memory locality. 

The intellegent approach calculated the values of each matrix cell utilizing 1D vector to represent the 2D matricies we multiplied. First these vectors were split up into
smaller tiles of 8x8 or 16x16 depending on the user's input. Then, using nested for loops, we iterate over the matrix as a whole as well as each tile internally. For each tile on the larger matricies we iterated through all the other tiles loading their rows and columns into 8 value vectors and multiply and
accumulated to the cells in the overall matrix that they contributed to.

The naive approach simply interated through every cell in the matrix and multiplied and added the full row and column from the input matricies all at once instead of
taking advantage of memory locality in the cache.

## Testing and Analysis

Testing consisted of the comparison between the intellegent and naive approaches

### Floating Point Matricies
| Matrix Size	| Time for Intellegent (s)	| Time for Naive (s)	| % Efficiency |
|---------------|---------------|---------------|------------|
| 100x100	| 0.000318 |	0.001799	| 176 |
| 1000x1000		|	0.098645 |	0.943665	| 957 |
| 2000x2000		|	2.645550 |	33.182484	| 1274 |
| 10000x10000		|	302.385559 |	N/A	| N/A |

### Short Matricies
| Matrix Size	| Time for Intellegent (s)	| Time for Naive (s)	| % Efficiency |
|---------------|---------------|---------------|------------|
| 256x256 | 0.031250 | 0.062500 | 3.33 |
| 512x512 | 0.437500 | 0.437500 | 0 |
| 1024x1024 | 2.546875 | 3.203125 | 44.2934783 |
| 2048x2048 | 20.593750 | 107.500 | 16.077092 |
| 4096x4096 | 171.703125 | 664.656250 | 20.5298261 |
| 8192x8192 |  |  |  |
| 10000x10000 |  |  |  |



## Conclusion

In this project we designed methods for large matrix multiplication that were extremely advantagous over more common methods. These methods utilize what we know about 
memory storage and how memory is loaded into the cache. We utilize this knowlege to take advantage of cache locality, or the fact that when something is loaded into
cache everything around it will be to. We can use this memory locality to design a matrix multiplication algorithm that takes advantage of this locality by splitting
up the matricies into tiles that will be loaded into cache together and then only using those tiles to do calculations before fetching a new tile. This way we can minimize cache misses which as we have seen significanlty cuts down on program time with increasing returns as the matricies get larger and larger.

These matrix multiplication algorithms have many applications, but the most useful is for the large datasets that you run into when doing any kind of machine learning
thus this program and others like it are necessary when desining and implementing new machine learning algorithms. 
