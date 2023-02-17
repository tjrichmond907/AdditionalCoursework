#**Thomas Richmond V00932371 CSC360 A3**
###Task 1

For this task I created helper functions which drive the progression `run_simulation()`. The basic flow of `run_simulation()` is that given the inputs, the function will enter a while loop which only exits when the **ready_q** and **event_list** are both empty and each iteration of that loop is in essence a single unit of time (tick). Within that loop the first helper function is found.

1. list\_check() 
	In this function the **event_list** is traversed and any node on the list which has an arrival_time less than or equal to the current tick is added to the ready_q and removed from the event list. The updated ready_q is returned to the call. 

2. idle\_check()
	In this function after the ready_q is updated or not in the `list_check()` function the idle check will determine if there infact was something placed on the ready queue and if not the system will display the idle messege for the current tick. If there is soemthing on the ready_q then the process proceeds.

3. This section which handles dlen and qlen operates as a switch board for the different combinations and inputs as dlen and qlen and keeps them organized so that qlen can run in batches then when it done dispatch messages are displayed. Within this switch board rr\_cycle() is called. 

4. rr\_cycle()
	in this function the task that is currently at the front of the ready q is operated on and it is determined if that task has reached its cpu_request time and if so a proper EXIT statement is the output, if not the standard use and request message is displayed. The funtion returns 1 or 0 to distinguish which type of message was output. 

5. This section is mostly cleanup, from the rr\_cycle() depending on the output number 1 or 0, the process will either increase qlen\_count and continue on until qlen is met, then it will reset the qlen and dlen switchboard, and either remove the task from the front to the back of the ready\_q or off the ready\_q if it is done. 

Depeneding on the location of many continue statements, the time is increased and the loop is re-entered again stating the process of the next time unit. 

###Task 2
For Task 2 I started to write a python script which would take in a .txt file in my case (out.txt) generated from the running of rrsim.c, and within the script the main funtion is to generate a graph of the average take wait and turnaround times of the ready\_q.
