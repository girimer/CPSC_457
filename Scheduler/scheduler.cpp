// you need to modify this file

#include "scheduler.h"
#include "common.h"
#include <algorithm>
#include <iostream>

// runs Round-Robin scheduling simulator
// input:
//   quantum = time slice
//   max_seq_len = maximum length of the reported executing sequence
//   processes[] = list of process with populated IDs, arrival_times, and bursts
// output:
//   seq[] - will contain the execution sequence but trimmed to max_seq_len size
//         - idle CPU will be denoted by -1
//         - other entries will be from processes[].id
//         - sequence will be compressed, i.e. no repeated consecutive numbers
//   processes[]
//         - adjust finish_time and start_time for each process
//         - do not adjust other fields
//

int64_t advance_time(std::vector<int> & RQ, std::vector<Process> & process, 
                    int64_t & curr_time, int64_t & quantum);


void condense(std::vector<int> & seq, int64_t max_len){
    std::vector<int> temp;
    uint i=1;
    int64_t counter=0;

    if(!seq.empty())
        temp.push_back(seq[0]);
    //std::cout<<"in while i: ";
    while(counter<max_len-1 && i < seq.size()){
        //std::cout<<i<<", ";
        if(seq[i-1] == seq[i]){
            i++;
        }
        else{
            temp.push_back(seq[i]);
            counter++;
            i++;
        }
    }
    seq.clear();
    seq=temp;
}

void simulate_rr(
    int64_t quantum, 
    int64_t max_seq_len,
    std::vector<Process> & processes,
    std::vector<int> & seq
) {std::vector<Process> process = processes;
    int64_t curr_time = 0;
    int64_t job_remaining = processes.size();
    std::vector<int> RQ;
    int64_t counter=0;

    seq.clear();

        while(true){
            //Adding job that arrived at current time
            for(uint i=0; i<process.size(); i++){
                if(process[i].arrival_time == curr_time){
                    RQ.push_back(i);
                }
            }

            if(job_remaining == 0){
                break;
            }
            
            if(RQ.empty()){
                seq.push_back(-1);
                
                //advance time to next process arrival time-1
                for (uint i=0; i<process.size(); i++){
                    if (process[i].arrival_time > curr_time){
                        curr_time = process[i].arrival_time - 1;
                        break;
                    }
                }
            }

            if(!RQ.empty()){
                    seq.push_back(RQ[0]);
                //recording start time if thsi is the first running process
                if(processes[RQ[0]].start_time < 0){
                    processes[RQ[0]].start_time= curr_time;
                }
                    process[RQ[0]].burst--;
                    counter++;

                int64_t before_advance_curr_time = curr_time;
                int64_t time_t_advance = quantum - counter;
                if(time_t_advance > process[RQ[0]].burst){
                    time_t_advance = process[RQ[0]].burst;
                }
               
                curr_time = curr_time + time_t_advance;

                //add jobs that arrived during advanced time and reduce bursts of current process
                if(before_advance_curr_time != curr_time){
                    process[RQ[0]].burst = process[RQ[0]].burst - time_t_advance;
                    counter = counter + time_t_advance;

                    for (uint i=0; i<process.size(); i++){
                        if(process[i].arrival_time >before_advance_curr_time &&
                            process[i].arrival_time <= curr_time){
                                RQ.push_back(i);
                            }
                    }
                }

                   //if bursts left for process now are zero.
                if(process[RQ[0]].burst == 0){
                    job_remaining--;
                    processes[RQ[0]].finish_time= curr_time+1;
                    RQ.erase(RQ.begin());
                    counter=0;
                }
                
                if(counter == quantum){
                std::rotate( RQ.begin(), RQ.begin() + 1, RQ.end() );
                counter=0;
                }
            }   

            //advance time if there is only 1 process in RQ
            if(RQ.size() ==1 && (seq.size() <= (uint)max_seq_len)){

                curr_time= curr_time + advance_time(RQ, process, curr_time, quantum);
            }

            bool start_time_flag= true;
            if(!RQ.empty() && (seq.size() > (uint)max_seq_len)){

                //check if all process has start time already
                for (uint i =0; i<RQ.size(); i++){
                    if(processes[RQ[i]].start_time<0){
                        start_time_flag=false;
                    }
                }
                if(start_time_flag){
                curr_time= curr_time + advance_time(RQ, process, curr_time, quantum);
                }
            }  
            curr_time++;
            condense(seq, max_seq_len);
        }     
}


int64_t advance_time(std::vector<int> & RQ, std::vector<Process> & process, 
                    int64_t & curr_time, int64_t & quantum){
    
        int64_t next_arrival=0;
        int64_t max_burst_advance=0;
        int64_t max_advance=0;
        bool no_more_process = false;

        for (uint i=0; i<process.size(); i++){
            if (process[i].arrival_time > curr_time){
                next_arrival = (process[i].arrival_time - curr_time);
                break;
            }
        }

        if(next_arrival == 0){
            no_more_process= true;
        }

        for (uint i=0; i<RQ.size(); i++){
            if(i == 0){
                max_burst_advance = process[RQ[i]].burst;
            }
            else{
                if(max_burst_advance > process[RQ[i]].burst){
                    max_burst_advance = process[RQ[i]].burst;
                }
            }
        }

    if(no_more_process){
        max_advance= max_burst_advance;
    }
    else{
        if(max_burst_advance < next_arrival){
         max_advance= max_burst_advance;
        }
        else {
            max_advance = next_arrival;
        }
    }

    //number of quantums to advance for each process
    //if no next arrival then advacne time to max_advacne * number of process and reduce each process burst
    //by max_advance
        if(no_more_process){
            max_advance= (max_advance/quantum)-1;
        }
        else{
        max_advance= ((max_advance/quantum)/RQ.size())-1;
        }
    
    if(max_advance<0){
        max_advance=0;
    }

    //reducing bursts for each process in RQ
    for (uint i=0; i<RQ.size(); i++){
           process[RQ[i]].burst = process[RQ[i]].burst - (max_advance*quantum);
        }

        return (max_advance*RQ.size()*quantum);   
}
