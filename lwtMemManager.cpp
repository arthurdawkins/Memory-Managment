/*
	Author: Arthur Dawkins 11/25/2019
	lwtMemManager is a memory allocation class
	init() creates the memory pool
	alloc(words) allocates a number of words taken as a param from the memory pool and returns the starting address of the allocated memory
	free(memStartAddress) takes the address returned by alloc and frees the specified block and joins it to any adjacent free blocks 
	display() displays every memory block in the memory pool in a easy to read format
*/
#include <iostream>

using namespace std;

class lwtMemManager{
private: 
	static const int memPoolSize = 1024;
	const int ALLOCATED = -1;
	const int FREE = 0;
	int memPool[memPoolSize];

public:

	void init () {// init function is the constructor 
		memPool[0] = ALLOCATED; //intialize memory pool as one big block  [-1,0,1018,0,0,.......0,0,1018,0,-1]
		memPool[1] = FREE;
		memPool[2] = memPoolSize - 6;
		memPool[memPoolSize-3] = memPoolSize - 6;
		memPool[memPoolSize-2] = FREE;
		memPool[memPoolSize-1] = ALLOCATED;
	}

	void * alloc(int words){
		if(words < 0 || words > memPoolSize - 6){
			return NULL;
		}
		int curr = 1;
		do{
			int size = memPool[curr+1];
			if(memPool[curr] == FREE && size >= words){ //check if block is free and large enough for request
				//ALLOCATE
				if(size > words+4){ //check if block is large enough to split
					memPool[curr] = ALLOCATED;
					memPool[curr+1] = words;
					memPool[curr+words+2] = words;
					memPool[curr+words+3] = ALLOCATED;
					memPool[curr+words+4] = FREE;
					memPool[curr+words+5] = (size-words-4);
					memPool[curr+(size-words-4)+2+words+4] = (size-words-4);
					memPool[curr+(size-words-4)+3+words+4] = FREE;
				}else{//if block is large enough to fit but not large enough to split
					memPool[curr] = ALLOCATED;
					memPool[curr+size+3] = ALLOCATED;
				}
				return &memPool[curr+2]; //return the address of the start of the array
			}else{//block was allocated or not large enough
				curr += size + 4; //jump to next block
			}
		}while(curr < memPoolSize-1);// keep going while pointer is still inside the memPool

		return NULL; //return -NULL to indicate that no free memory of the size required was found. 
	}

	void free(void *memStartAddress){
		int index = ((int*)memStartAddress) - &memPool[0];
		int preFlag = index -2;
		int size = memPool[index-1];
		int suFlag = index +size + 1;
		if(memPool[preFlag] == ALLOCATED && memPool[suFlag] == ALLOCATED && size == memPool[index+size]){ //making sure the values we found for prefix, sufix and size make sense 
			memPool[preFlag] = FREE;
			memPool[suFlag] = FREE;
			int lSize = 0;
			if(memPool[index-3] == FREE){ //if block to the left is free
				lSize = memPool[index-4];
				memPool[index-5-lSize] = size+lSize+4;
				memPool[index+size] = size+lSize+4;
			}
			if(memPool[index+size+2] == FREE){ // block to the right is free
				int rSize = memPool[index+size+3];
				if(lSize > 0){//if we freed a left block as well
					memPool[index-5-lSize] = size+lSize+8+rSize;
					memPool[index+size+4+rSize] = size+lSize+8+rSize;
				}else{// if didn't free the left block
					memPool[index-1] = size+4+rSize;
					memPool[index+size+4+rSize] = size+4+rSize;
				}
				
			}
		}
		
	}

	void display(){
		int index = 1;
		int blockCount = 1;

		cout << endl;
		cout << "------------------------memPool start-----------------------" << endl;
		cout << endl;
		do{
			char* allocated = "FREE";
			if(memPool[index] == ALLOCATED) allocated = "ALLOCATED"; 
			cout << "block number: " << blockCount++ << endl;
			cout << allocated << endl;
			cout << "memory address: " << &memPool[0]+index+2 << endl;
			cout << "Size: " << memPool[index+1] << endl;
			cout << "offset start: " << index+2 << endl;
			cout << "offset end: " << index+memPool[index+1]+2 << endl;
			index += memPool[index+1]+4;
			cout << endl;
		}
		while(index < memPoolSize-1);
		cout << endl;
		cout << "------------------------memPool end-----------------------" << endl;
		cout << endl;
	}
};

