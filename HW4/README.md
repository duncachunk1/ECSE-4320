# Dictionary Compression - Project 4

The objective of this assignment was to utilize our knowlege of data compression and data storage structures to create a program that could take in a column of text
and create an encoded hashtable that can be searched using either whole or partial search terms

## Installation and Environment

To Install please make sure that you have downloaded the full folder

System specs:
8 core 11 gen intel core i7 @ 2.3GHz with 32 GB of ram
Ubuntu 22.04.1 LTS

## Compilation

Use the following command to compile the program

```
g++ -mavx -march=native -mfpmath=both -O3 -pthread -g main.cpp -o a.out
```
or if you are having trouble compiling
```
g++ -mavx512f -mavx512vl -mavx512bw -march=native -mfpmath=both -O3 -pthread -g main.cpp -o a.out
```
## Execution

Once you have compiled you may run with the command
```
./a.out <inputfile> #threads
```
we have provided smallFile.txt and bigFile.txt for ease of use, but our code also works with the professor provided file

## About the Code

This project uses compression and then dictionary encoding to compress a large text file and then use a hash function to generate unique keys for every unique index
we then store these unique keys in a hashtable.  Finally we use a recursive search function that utilizes the <vector.h> library which uses AVX commands to quickly and efficently load large vectors of keys into cache and then check the searched key against the loaded vector.

## Code Structure

Our code has three main sections: 

First is sequential compression which uses multiple threads that are each given a vector full of column elements to compress, counts the number of sequential characters and uses them to compress individual items, and then joins the threads togethers creating a list of compressed key index pairs.

Second is a hash funciton, we used a modulus, which gives us a unique hash key from our funciton input. We then store this hash key along with the original item in our hash table. We also do some simple collision correction which is on the off chance that two different items have the same key value we create a linked list where each item with the same key points to the next item that has the same hash key.

Third we implemented two search functions, one that uses AVX instructions to load in a vector of key values and then compares the searched key with the loaded chunk of keys and finds where is there is overlap. If there is significant overlap we look for a match for that key and if we find one we return the key. We also have a prefix search function which uses the same methodology but returns any key that contains the partial search term.

For comparason we also implemented a naive search function for comparason which simply searches for the correct item by iterating through the column.

## Testing and Analysis

Testing consisted of the comparison between the intellegent and naive search approaches as well as encoding time.

### Search times
#### 7KB File
##### Encoded Search Time: 3ms

##### Vanilla Search Time
| Threads |  Time for Naive(ms) | Efficency % |
|---------------|---------------|---------------|
| 1	| 49 |  1633 |
| 2	| 38 |  1266 |
| 4	| 29 |  966 |
| 8	| 49 |  1633 |
| 16  | 55 |  1833 |

#### 6.34MB File
##### Encoded Search Time: 7ms

##### Vanilla Search Time
| Threads |  Time for Naive(s) | Efficency % |
|---------------|---------------|---------------|
| 32	| 74.1 |  105.6 * 10^2 |
| 64	| 80.4 |  114.9 * 10^2 |
| 128	| 79.5 |  113.5 * 10^2 |
| 256	| 80.4 |  114.9 * 10^2 |
| 512	| 83.0 |  118.6 * 10^2 |

### Encode times
#### 7KB File
| Threads | Time for Encoding |
|---------------|---------------|
| 1 |	53ms +/- 1.6ms	|
| 2 |	38.2ms +/- 2.03ms	|
| 4 |	32.8ms +/- 2.13ms	|
| 8 |	46.0ms +/- 2.55ms	|
| 16 |	57.6ms +/- 2.81ms	|

#### 6.34MB File
| Threads | Time for Encoding |
|---------------|---------------|
| 32 |	133s  +/- 8.26s	|
| 64 |	156s  +/- 8.11s |
| 128 |	160s  +/- 9.03s |
| 256 |	161s	+/- 4.48s |
| 512 |	176s  +/- 10.3s	|


## Conclusion

In this project we designed methods for dictionary compression and encoding that were extremely advantagous over more common methods. These methods utilize what we know about memory storage and how memory is loaded into the cache. We utilize this knowlege to take advantage of a data structure known as a hashtable, which is a list of key item values where each item has a single unique key produced by a hash function. We can use this hashtable to design a compression, storage, and search algoritm that takes advantage of this datastructure by compressing all the items with multiple threads and using a simple hash function that will allow us to more easily search for specific items in the list by referencing the unique keys. This way we can minimize cache misses which as we have seen significanlty cuts down on program time with increasing returns as the column gets larger and larger.

This Dictionary compresison algorithm has many applications, but the most useful is for the creation of large databases from even larger amounts of data minimizing the amout of time it takes to fetch items from this database.
