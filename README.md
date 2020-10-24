# Asst1
The goal of this program is to recreate the standard C functions malloc and free to allocate and free memory only from a predefined block of memory of 4096 bytes. The memory block is made up of two types of partitions- metadata, and data. The metadata partitions are structures with 2, 4-byte integers stored inside: status of the data (CLOSED or OPEN), and length of the data. 
Pointers to data partitions marked CLOSED by metadata->status will not be returned by malloc, whereas partitions marked OPEN will be returned, as long as they have at least enough space for the requested amount of bytes. After each metadata partition is its respective data partition. 
When attempting to free data, the user must give a valid memory address pointing to the start of a data partition. myfree errors if this does not happen, and if it does, it looks for a valid metadata partition in the block before it. If there is a valid metadata partition and it is marked as CLOSED, we set its status to OPEN so that it is now free for the user to use and return to the caller. 
## Great Features
* Contiguous, open blocks of memory are merged together in linear runtime in order to possibly provide space for larger data.
* Errors are printed with useful information for the user.
* Partitioning of leftover free space only happens if we have enough space for metadata + at least 1 byte of data.