Runs Round-Robin scheduling simulator

input:
  - quantum = time slice
  - max_seq_len = maximum length of the reported executing sequence
  - processes[] = list of process with populated IDs, arrival_times, and bursts
  - 
output:
  - seq[] - will contain the execution sequence but trimmed to max_seq_len size
        - idle CPU will be denoted by -1
        - other entries will be from processes[].id
        - sequence will be compressed, i.e. no repeated consecutive numbers
  - processes[]
        - adjust finish_time and start_time for each process
        - do not adjust other fields
