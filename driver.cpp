/*
	Author: Arthur Dawkins 11/25/2019
	A driver program for use with lwtMemManager
	calls alloc 10 times with random requests between 1 to 250 words and records all of the returned addresses
	after 10 allocation requests all of the memory is freed in a random order. 
*/
#include "lwtMemManager.cpp"

int main(void ){

	srand(time(NULL)); // seeding rand with current time to get randomer random numbers
	lwtMemManager mm;
	mm.init();
	cout << "starting mem" << endl;
	mm.display();
	
	int numAllocCalls = 10;//number of calls to make
	void *addresses[numAllocCalls];
	int addressIndex = 0;

	for(int i = 0; i < numAllocCalls;i++){
		int rnd = rand() % 250 + 1;
		void *ad = mm.alloc(rnd); // random allocation calls
		if(ad != 0){
			addresses[addressIndex++] = ad;
			cout << "alloc("<<rnd<<") returned " << ad << endl;
		}else{
			cout << "alloc("<<rnd<<") returned " << ad << " no memory allocated" << endl;
		}
		
		mm.display();
	}
	cout << endl;

	for(int i = 0; i < addressIndex;i++){//printing the addresses returned by alloc
		cout << addresses[i] << ", ";
	}
	cout << endl;

	
    for(int i = addressIndex-1; i > 0; i--){//suffle the addressess array for random removal
     	int j = rand() % i;
     	void* temp = addresses[i];
     	addresses[i] = addresses[j];
     	addresses[j] = temp; 
    }
	

	for(int i = 0; i < addressIndex;i++){//call free on every address in the address array
		if(addresses[i] > 0){
			mm.free(addresses[i]);
			cout << "free("<<addresses[i]<<")" << endl;
			mm.display();
		}
	}
	cout << endl;
	
	
	

}