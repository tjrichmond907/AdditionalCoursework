/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */

sem_t readwrite_mutex, lock_mutex;
int read_count;
static resource_t data;
char lab[MAX_LABEL_LEN];

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
	init_resource(&data,lab); 
	 sem_init(&readwrite_mutex, 0, 1);
	 sem_init(&lock_mutex, 0, 1); 
	 read_count = 0;
}


void rw_read(char *value, int len) {
    
	sem_wait(&lock_mutex);		//lock from other readers
	read_count++;
	if(read_count == 1) {		//checks if there are other readers
	 sem_wait(&readwrite_mutex);	//locks the resouce from writers
	 }
	sem_post(&lock_mutex);		//
	read_resource(&data, value, len);//Critical section
	sem_wait(&lock_mutex);
	read_count--;
	if(read_count == 0) {
		sem_post(&readwrite_mutex);
       	 }
	sem_post(&lock_mutex);	
}


void rw_write(char *value, int len) {
    
	sem_wait(&readwrite_mutex);		//lock from readers/writers
	write_resource(&data, value, len); 	//crititcal section
	sem_post(&readwrite_mutex);		//release to readers
 
}

//works cited:
//Sliberschatz, A. , Galvin, P.B., Gagne, G. (2018). Synchronization Examples. In Fowley, D. (Eds.), Operating Systems Concepts (pp. 290 - 293). L. Rosatone
