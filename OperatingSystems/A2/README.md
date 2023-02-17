#**Thomas Richmond V00932371 CSC360 A2**
###Task 1
In the implementation of task 1 I reference pseudocode found in the course textbook, Operating Systems Concepts. This task implemented in **rw.c** completes the three functions given using semaphore.h. 

1. initialize\_readers\_wirters()
	In this function, all appropriate shared structures such as resource, semaphore, and counters are initialized and the sempahores are set to 1 and count to 0. This ensures that only one thread at a time can posses the semaphore. 

2. rw\_read() 
	In this function, the first block of code between the first set of `sem_wait(&lock_mutex)` and `sem_post(&lock_mutex)` checks and prevents other readers from accessing the resource to be read. In the next section of the function at `if(read_count == 0)`, the program is checking for any additional readers before unlocking the mutex for possible writing to the resource. 

3. rw\_write()
	In this function, the sempahore is locking and unlocking the sempaphore around the critical section of the `write_resource()` function. This will allow only one writer at a time to access the resource and write to it. 

In this task the functions are implemented in such a way that readers will be given priority.


###Task 2  

For task 2 the solution was derived from the lecutre slides and modified to have an outside \"generation\" control rather than inside. The basic structure of the code is as follows: 

1. control the group of Boba Fetts entering by checking if a shared count variable is greater than or equal to the size of the group as passed. -> wait if it is. 

2. Determine if the incoming thread is the first or the last Boba to arrive, and if they are, determine what the `meet_type` is; meet first or meetlast.
if these conditions are true the value that that Boba has is written to the shared resource to be read by the other n-1 Fetts, after a broadcast.

3. Determine if a write has happened yet and if it has then the readers are able to read the resource and broadcast. 

4. The cleanup of the current group of Boba's is completed, shared variables are retrurned to their initial state and the next batch is signaled via a broadcast.  


\*All works have been cited in the files where the work they cite is used. 
