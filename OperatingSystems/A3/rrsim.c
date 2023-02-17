#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_BUFFER_LEN 80

taskval_t *event_list = NULL;
int len =0;

void print_task(taskval_t *t, void *arg) {
    printf("task %03d: %5d %3.2f %3.2f\n",
        t->id,
        t->arrival_time,
        t->cpu_request,
        t->cpu_used
    );  
}


void increment_count(taskval_t *t, void *arg) {
    int *ip;
    ip = (int *)arg;
    (*ip)++;
}

//debugger
void print_rq(taskval_t *prq) {
	taskval_t *tmp = peek_front(prq);
	while(tmp!=NULL){
		printf("id: %d, arrival: %d, req: %f\n", tmp->id, tmp->arrival_time, tmp->cpu_request);
		tmp = tmp->next;
	}
	return;
}

//function to determine if all tasks have been processed
int is_empty(taskval_t* e_list, taskval_t* r_list){
	taskval_t* emp;
	taskval_t* rmp;
	rmp = peek_front(r_list);
	emp = peek_front(e_list);
	if(emp == NULL && rmp == NULL) {
		return 0;
	} else {
		return 1;
	}
}

//updates the ready_q with nodes that are now eligible to be running 
taskval_t* list_check(int cur_tick, taskval_t *rq) {
	taskval_t *lc_temp;
	lc_temp = peek_front(event_list);
	while(lc_temp != NULL) {
		if(lc_temp->arrival_time >  cur_tick) {
			break;
		} else {
			
			lc_temp = peek_front(event_list);
			event_list = remove_front(event_list);
			rq = add_end(rq, lc_temp);
			
			lc_temp = peek_front(event_list);
			continue;
		}
	}
	return rq;
}


//checks if anything has been added to ready_q, prints idle for this tick if not
int idle_check(int ic_tick, taskval_t *ic_rq) {
	taskval_t *ic_temp;
	ic_temp = peek_front(ic_rq);
	if(ic_temp == NULL) {
		printf("[%05d] IDLE\n", ic_tick);
		return 1;
	} else {
		return 0;
	}
}


//completes the round robin cycle by checking cpu data
int rr_cycle(int rr_tick, taskval_t* rr_rq) {
	taskval_t *rr_temp;
	rr_temp = peek_front(rr_rq);

	if((rr_temp->cpu_used ) >= rr_temp->cpu_request) {
		rr_temp->finish_time = rr_tick;
		printf("[%05d] id=%05d EXIT w=%.2f ta=%.2f\n", rr_tick,
		rr_temp->id, (rr_temp->finish_time - rr_temp->arrival_time -
		rr_temp->cpu_request), (float)(rr_temp->finish_time -
		rr_temp->arrival_time));

		return 0;
	} else {
		printf("[%05d] id=%05d req=%.2f used%.2f\n", rr_tick, rr_temp->id, rr_temp->cpu_request, rr_temp->cpu_used);
		rr_temp->cpu_used++;
		return 1;
	}
}
			

void run_simulation(int qlen, int dlen) {

	int tick = 0, dlen_count = 0, qlen_count = 1, rr_val = 0;
	taskval_t* ready_q = NULL, *temp;

	while(is_empty(event_list, ready_q)) {
		
		ready_q = list_check(tick, ready_q);

		if(idle_check(tick, ready_q)) {
			tick++;
			continue;
		} else {
			if(dlen == 0) {
				if((qlen_count -1) == 0 || qlen < qlen_count) {
					printf("[%05d] DISPATCHING\n", tick);
					goto label;
				} else {
					goto label;
				}
			} else {
				if(dlen_count < dlen) {
					printf("[%05d] DISPATCHING\n", tick);
					dlen_count++;
					tick++;
					continue;
				} else {
label:
					rr_val = rr_cycle(tick, ready_q);
					if(rr_val && qlen_count < qlen) {
						qlen_count++;
						tick++;
						continue;
					} else{
						qlen_count = 1;
						dlen_count = 0;
						if(rr_val) {
							temp = peek_front(ready_q);
							ready_q = remove_front(ready_q);
							ready_q = add_end(ready_q, temp);
							tick++;
							continue;
						} else {
							ready_q = remove_front(ready_q);
							tick++;
							continue;
						}

					}
				}
			}
		}
	}
	return;

}

		

int main(int argc, char *argv[]) {
    char   input_line[MAX_BUFFER_LEN];
    int    i;
    int    task_num;
    int    task_arrival;
    float  task_cpu;
    int    quantum_length = -1;
    int    dispatch_length = -1;

    taskval_t *temp_task;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quantum") == 0 && i+1 < argc) {
            quantum_length = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--dispatch") == 0 && i+1 < argc) {
            dispatch_length = atoi(argv[i+1]);
        }
    }

    if (quantum_length == -1 || dispatch_length == -1) {
        fprintf(stderr, 
            "usage: %s --quantum <num> --dispatch <num>\n",
            argv[0]);
        exit(1);
    }


    while(fgets(input_line, MAX_BUFFER_LEN, stdin)) {
        sscanf(input_line, "%d %d %f", &task_num, &task_arrival,
            &task_cpu);
        temp_task = new_task();
        temp_task->id = task_num;
        temp_task->arrival_time = task_arrival;
        temp_task->cpu_request = task_cpu;
        temp_task->cpu_used = 0.0;
        event_list = add_end(event_list, temp_task);
    }

#ifdef DEBUG
    int num_events;
    apply(event_list, increment_count, &num_events);
    printf("DEBUG: # of events read into list -- %d\n", num_events);
    printf("DEBUG: value of quantum length -- %d\n", quantum_length);
    printf("DEBUG: value of dispatch length -- %d\n", dispatch_length);
#endif

    run_simulation(quantum_length, dispatch_length);

    return (0);
}
