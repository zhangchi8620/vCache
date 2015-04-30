/* Virtual Cache */

//#include "block.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef struct BLOCK_STRUCT{
	long int addr;
	bool dirty;
	int LRU;
}block;

typedef struct CACHE_STRUCT{
	int blk_num;
	int assoc;
	float time_hit;
	int blk_index_size;
	int blk_tag_size;
	int hit_count;
	int miss_count;
	block *blk[];
}cache;


void printInfo(){
	printf("\n***********************************************************************\n");
	printf("Usage:\n");
	printf("\tTest file:\t\tASCII_sample or ASCII_many\n");
	printf("\t# of caches:\t\t1-3\n");
	printf("\tWrite Policy:\t\t0 for write back, 1 for write through\n");
	printf("\tWord size:\t\tshould be power of 2, in bytes\n");
	printf("\tBlock size:\t\tshould be power of 2, in words\n");
	printf("\t# of blocks:\t\tmust be power of 2\n");
	printf("\tAssociativity:\t\tmust be power of 2\n");
	printf("***********************************************************************\n\n");
}


int Deci2Bina(int x){
	int s = 0;
	x--;
	while (x/2){
		x = x/2;
		s++;
	}
	return ++s;
}

int maxCounter(cache *L, int head){
	int max = L->blk[head]->LRU;
	int  oldest = head;
	for(int i = head; i < head + L->assoc; i++){
		if(max < L->blk[i]->LRU){
			max = L->blk[i]->LRU;
			oldest = i;
		}
	}
	return oldest;
}


int find(long int addr, cache *L){
	for(int i = 0; i < L->blk_num; i++){	
		if(addr == L->blk[i]->addr){
			return i;
		}
	}
	return -1;
}

int replace(long int addr, cache *L){
	int r = 0;
	int index = addr & ((int)pow(2, L->blk_index_size) - 1);

	if(L->assoc == L->blk_num)
		r = rand() % L->blk_num;
	else if(L->assoc == 1){
		r = maxCounter(L, 0);
	}
	else{
		int set = index % (L->blk_num / L->assoc);
		r = maxCounter(L, set * L->assoc);
	}
	return r;
	}

void printCache(cache *L){
	for(int i = 0; i < L->blk_num; i++)
		printf("block %d: %ld\n", i, L->blk[i]->addr);
}


int main(){

	/*************** Reading from ASCII ***************/
	printInfo();
	long int LINE_LENGTH = 0;
	int ADDR_LENGTH = 0;
	string s[1000000];
	char filename[100];

	printf("Input test file:\t");
	scanf("%s", filename);
	ifstream myfile(filename);
	
	if(!myfile.is_open()){
		printf("Error: Invalid file. Please try again.\n");
		return 0;
	}
	
	
	if(myfile.is_open())
	{
		long int i = 0;
		while(getline(myfile, s[i])){
			i++;
		}
		myfile.close();
		LINE_LENGTH = i;
		ADDR_LENGTH = s[0].size() - 2;
		
	}
	else
		cout << "Unable to open file";

	//Fetch address and R/W bit 		
	char addr[LINE_LENGTH][ADDR_LENGTH+1];
	memset(addr, 0, LINE_LENGTH*ADDR_LENGTH*sizeof(char));
	char flag[LINE_LENGTH][2];
	memset(flag, 0, LINE_LENGTH*2*sizeof(char));

	size_t length;
	for(int i = 0; i < LINE_LENGTH; i++){
		length = s[i].copy(addr[i], ADDR_LENGTH, 0);
		addr[i][length] = '\0';
		length = s[i].copy(flag[i], 1, s[i].size()-1);
		flag[i][length] = '\0';
	}
	
	/*************** User input ***************/
	int word_size;
	int blk_size;
	float time_mem;
	int write_policy;
	int cache_num;

	printf("\nPlease configure vCache >>>>>>\n");
	printf("Number of caches:\t\t");
	scanf("%d", &cache_num);
	while(cache_num > 3 || cache_num < 1){
		printf("Error: Invalid input\n");
		printInfo();
		printf("Please try again\n");
		scanf("%d", &cache_num);
	}
	printf("Write policy:\nwrite back=0, write through=1:\t");
	scanf("%d", &write_policy);
	while(write_policy != 0 && write_policy != 1){
		printf("Invalide input");
		printInfo();
		printf("Please try again\n");
		scanf("%d", &write_policy);
	}

	printf("Word size(Byte):\t\t");
	scanf("%d", &word_size);
	printf("Block size (words):\t\t");
	scanf("%d",&blk_size);
	printf("Read/Write time to Memory(ns):\t");
	scanf("%f", &time_mem);

	cache *L[cache_num];
	for(int i = 0; i < cache_num; i++){
		L[i] = (cache *)malloc(10000 * sizeof(cache));
		printf("\nL%d cache:\n", i+1);
		
		printf("Number of blocks:\t\t");
		scanf("%d", &L[i]->blk_num);
		while(L[i]->blk_num &(L[i]->blk_num - 1)){
			printf("Invalide input");
			printInfo();
			printf("Please try again\n");
			scanf("%d", &L[i]->blk_num);
		}


		printf("Associativity:\t\t\t");
		scanf("%d", &L[i]->assoc);
		while(L[i]->assoc &(L[i]->assoc - 1)){
			printf("Invalide input");
			printInfo();
			printf("Please try again\n");
			scanf("%d", &L[i]->assoc);
		}
	
		printf("Hit time(ns):\t\t\t");
		scanf("%f", &L[i]->time_hit);
	
		L[i]->hit_count = 0;
		L[i]->miss_count = 0;
		for(int j = 0; j < L[i]->blk_num; j++){		
			L[i]->blk[j] = (block *)malloc(10000 * sizeof(block));	
			L[i]->blk[j]->addr = -1;
			L[i]->blk[j]->dirty = false;
			L[i]->blk[j]->LRU = 0;
		}
	}

/*************** Initialize Caches ***************/

	for(int i = 0; i < cache_num; i++){	
		for(int j = 0; j < L[i]->blk_num; j++){	
		}
	}

	int BLK_OFFSET_LENGTH, BLK_ADDR_LENGTH;
	BLK_OFFSET_LENGTH = Deci2Bina(blk_size * word_size);
	BLK_ADDR_LENGTH = ADDR_LENGTH * 4 - BLK_OFFSET_LENGTH;

	long int blk_addr[LINE_LENGTH];

	for(int i = 0; i < LINE_LENGTH; i++){
		blk_addr[i] = strtol(addr[i], NULL, 16) >> BLK_OFFSET_LENGTH;
	}

	for(int i = 0; i < cache_num; i++){
		if (L[i]->assoc == L[i]->blk_num)
			L[i]->blk_index_size = 0;
		else if (L[i]->assoc == 1)
			L[i]->blk_index_size = ADDR_LENGTH * 4 - BLK_OFFSET_LENGTH;
		else
			L[i]->blk_index_size = Deci2Bina (L[i]->blk_num / L[i]->assoc);
		L[i]->blk_tag_size = BLK_ADDR_LENGTH- L[i]->blk_index_size;
	}

	/*************** Search Caches ***************/	
	double AMAT = 0;
	int rep = 0;
	for(int i = 0; i < LINE_LENGTH; i++){
		for(int j = 0; j < cache_num; j++){	
			
			for(int k = 0; k < L[j]->blk_num; k++){
				L[j]->blk[k]->LRU ++;
			}
			
			int hit = find(blk_addr[i], L[j]);
			if (hit >= 0){
				L[j]->hit_count += 1;
				if(strcmp(flag[i], "W") == 0 && write_policy == 0)
					L[j]->blk[hit]->dirty = true;
				break;
			}
			else{
				
				L[j]->miss_count += 1;

					if(write_policy == 1){
						rep = replace(blk_addr[i], L[j]);
						L[j]->blk[rep]->addr = blk_addr[i];				
						L[j]->blk[rep]->LRU = 0;
					}
					else{
						rep = replace(blk_addr[i], L[j]);
						int temp = rep;
						if(L[j]->blk[rep]->dirty == false){
							L[j]->blk[rep]->addr = blk_addr[i];
							L[j]->blk[rep]->LRU = 0;
						}
						else{
							L[j]->blk[rep]->dirty = false;
							for(int mm = j; mm < cache_num; mm++){
								rep = replace(blk_addr[i], L[mm]);
								L[mm]->blk[rep]->addr = blk_addr[i];
								L[mm]->blk[rep]->LRU = 0;
								L[mm]->blk[rep]->dirty = false;
							}
						}
						
						if(strcmp(flag[i],"W")==0){
							L[j]->blk[temp]->dirty = true;
							break;
						}
				
					}
			}
			
		}
			
	}

	/*************** Calculate performance ***************/
	printf("\n>>>>>> Performance\n");
	for(int i = 0; i < cache_num; i++){
		double miss_rate = ((double)L[i]->miss_count)/((double)L[i]->hit_count+(double)L[i]->miss_count);
		double hit_rate = ((double)L[i]->hit_count)/((double)L[i]->hit_count+(double)L[i]->miss_count);
		
		printf("L%d Cache hit: %d, miss: %d; hit rate: %f, miss rate: %f\n", i+1, L[i]->hit_count, L[i]->miss_count, hit_rate, miss_rate);
	}

	double miss_penalty = time_mem;
	for(int i = cache_num - 1; i >= 0; i--){
		miss_penalty = L[i]->time_hit +((double)L[i]->miss_count)/((double)L[i]->hit_count+(double)L[i]->miss_count)*miss_penalty;
	}
	AMAT = miss_penalty;
	printf("AMAT = %f\n", AMAT);
	for(int i = 0; i < cache_num; i++){		
		for(int j = 0; j < L[i]->blk_num; j++)
			free(L[i]->blk[j]);
		free(L[i]);
	}
}
