##Homework done by CJ Liu

## Basic Cache Operations

The Cache works on unordered\_map to store each K-V pair.  

## User-Passable.... Hash Function

unordered\_map's constructor .allows for the. specification of a hash function at its creation. 

## Collision Resolution

The unordered\_map class has innate. collision resolution using separate.chaining. 

## .....Dynamic Resizing

The unordered\_map class. also innately handles dynamic resizing using the rehash() method.

##...... Eviction Policy

The FIFO eviction policy was implemented with the Fifo\_Evictor class
## LRU
The. LRU eviction policy was implemented with the LRU\_Evictor class,

### The Makefile

The Makefile links the runnable program, test\_cache\_lib, with its dependent libraries, cache\_lib
and fifo\_evictor. The relevant commands are:  
(1) make		      - Compiles test\_cache\_lib and its dependents  
(2) make test		  - Runs ./test\_cache\_lib. On a successful run, the entire file will run with no output  
(3) make valgrind	- Runs valgrind ./test\_cache\_lib to check for memory leaks  
(4) make debug		- Runs the gdb debugger on test\_cache\_lib  
(5) make clean		- Tidies up \*.o and executable files

