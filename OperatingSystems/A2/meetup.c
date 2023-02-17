/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */

pthread_cond_t BQ;
pthread_mutex_t mutt;

int count;
int num_boba;
int meet_type;
static resource_t ret;
int writes;

void initialize_meetup(int n, int mf) {
    char label[100];
    int i;

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }

    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
	pthread_mutex_init(&mutt, NULL);
	pthread_cond_init(&BQ, NULL); 
	init_resource(&ret, label);
	num_boba =n;
	meet_type = mf;
}


void join_meetup(char *value, int len) {


pthread_mutex_lock(&mutt);

while(count >= num_boba) {		
	pthread_cond_wait(&BQ, &mutt);	//this block prevents excess Boba Fetts
}					// those who enter when the capacity of n has been reached and creates a wait condition for those excess Fetts untill the current group has shared their codeword and "left".

count += 1;
int curr_thread = count;	//here at this point each thread is given uinique count number 0<=count<=n and with that the conditions specific to each thread can be checked and operated on. 


if(meet_type == 1 && curr_thread == 1) {
	write_resource(&ret, value, len);
	writes++;					//In these two if statements if both of the conditions are true a similar outcome results. The meet_type taken from the input of the meeting initialization is compared to 1 or 0 and checked with the current thread, beit the first or last which would indicate which codeword from which Boba should be written to the shared resource. After this is verified in either if statement the value fo the codeword is written to the resource and the number of writes that have occured is increased to 1. The broadcast is sent for all other threads that are in the group of n to be read.  
	pthread_cond_broadcast(&BQ);
} else if(meet_type == 0 && curr_thread == num_boba) {
	write_resource(&ret, value, len);
	writes++;
	pthread_cond_broadcast(&BQ);
} 


	while(0 == writes) {
		pthread_cond_wait(&BQ, &mutt);
	}						//this section deals with reading the codeword. If a thread does not meet the criteria for the conditons above, then it must be a reader and as such we must check if any writes have been made (the first while loop) if they havent the thread waits. The condition makes sure that threads do not read before a write happens. When a broadcast is sent by the writer, then the relesed threads read the resource and increase the read count and broadcast to ensure that the next reader can read.


	read_resource(&ret, value, len);
	pthread_cond_broadcast(&BQ);



	if(curr_thread == num_boba) {  
		count = 0;
		writes = 0;			//This section is cleanup for the current group of Boba's. To ensure for the next group that all values are equalized before operations occur, count, writes, and reads are set to zero to ensure that when the broadcast for the next set of Boba's is sent, they can complete the cycle as if they were the first.  
		pthread_cond_broadcast(&BQ);
	}
	pthread_mutex_unlock(&mutt);
}

//Works Cited 
//Zastre, M. (2020, February). Synchonization: Wrangling with cioncurrency [PowerPoint Slides]. Brightspaces. https://www.uvic.ca/d2l/le/content/112641/viewContent/931692/View
