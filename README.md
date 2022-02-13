# CPSC_471--Fast-palindrome
Program reads file from stdin and outputs longest palindrome. Optimized to reduce read operations. 
Tests were performed against slow-pali implementation provided by instructor. 

Test1:

      Slow-pali
      girimer.singh@ms160-3we:~/Documents/CPSC_457/Lab_1$ strace -c ./slow-pali < t4.txt
      Longest palindrome: redder
      % time  seconds     usecs/call  calls     errors  syscall
    ------  ---------   ----------    --------  ------  ------
      100.00  11.563265     2         5767198           read
      0.00    0.000034      0         58          53    openat
      0.00    0.000027      3         9                 mprotect
      0.00    0.000023      1         22                mmap
      0.00    0.000009      0         16          9     newfstatat
      0.00    0.000008      2         4                 pread64
      0.00    0.000006      6         1                 write
      0.00    0.000006      6         1                 munmap
      0.00    0.000005      1         3                 brk
      0.00    0.000002      0         5                 close
      0.00    0.000002      1         2           1     arch_prctl
      0.00    0.000000      0         1           1     access
      0.00    0.000000      0         1                 execve
    ------  ---------   ----------    --------  ------  ------
      100    11.563387     2         5767321     64    total
    
      Fast-pali
      girimer.singh@ms160-2wd:~/Documents/CPSC_457/Lab_1$ strace -c ./fast-pali < t4.txt
      Longest palindrome: redder
      % time seconds usecs/call calls errors  syscall
      ------ ------- ---------- ----  -----   ------ 
      66.98 0.001546    128     12            munmap
      9.10  0.000210    17      12            read
      7.67  0.000177    3       58    53      openat
      6.07  0.000140    140     1             execve
      4.59  0.000106    3       33            mmap
      1.91  0.000044    2       16    9       newfstatat
      1.65  0.000038    4       9             mprotect
      0.69  0.000016    1       11            brk
      0.52  0.000012    2       5             close
      0.35  0.000008    2       4             pread64
      0.22  0.000005    2       2     1       arch_prctl
      0.17  0.000004    4       1     1       access
      0.09  0.000002    2       1             write
      ------ ------- ---------- ----  -----   ------ 
      100   0.002308    13      165   64      total
      
Test 2:

      Slow-pali
      girimer.singh@ms160-3we:~/Documents/CPSC_457/Lab_1$ strace -c ./slow-pali < t3.txt
      Longest palindrome: ___o.O.o___
      % time seconds usecs/ calls errors syscall
                      call 
      ----  --------  ---   ----  -----   ------
      34.06   0.000171  2   58    53      openat
      25.50   0.000128  128 1             execve
      17.33   0.000087  3   22            mmap
      7.97    0.000040  4   9             mprotect
      7.37    0.000037  2   16    9       newfstatat
      1.99    0.000010  0   53            read
      1.99    0.000010  2   5             close
      1.59    0.000008  2   4             pread64
      0.80    0.000004  4   1     1       access
      0.80    0.000004  2   2     1       arch_prctl
      0.60    0.000003  1   3             brk
      0.00    0.000000  0   1             write
      0.00    0.000000  0   1             munmap
      ----  --------  ---   ----  -----   ------
      100     0.000502  2   176   64      total
      
      Fast-pali
      girimer.singh@ms160-2wd:~/Documents/CPSC_457/Lab_1$ strace -c ./fast-pali < t3.txt
      Longest palindrome: ___o.O.o___
      % time seconds usecs/ calls errors syscall
                      call 
      ----  --------  ---   ----  -----   ------
      0.00  0.000000  0     7             read
      0.00  0.000000  0     1             write
      0.00  0.000000  0     5             close
      0.00  0.000000  0     22            mmap
      0.00  0.000000  0     9             mprotect
      0.00  0.000000  0     1             munmap
      0.00  0.000000  0     3             brk
      0.00  0.000000  0     4             pread64
      0.00  0.000000  0     1       1     access
      0.00  0.000000  0     1             execve
      0.00  0.000000  0     2       1     arch_prctl
      0.00  0.000000  0     58      53    openat
      0.00  0.000000  0     16      9     newfstatat
      ----  --------  ---   ----  -----   ------
      100   0.000000  0     130     64    total
