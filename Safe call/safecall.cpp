#include "safecall.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <sys/wait.h>
#include <thread>
#include <cstring>
 ====================================================================

int safecall(int i)
{
  int result;
  const char *path = "/tmp/Output.txt";

  auto start_time = std::chrono::steady_clock::now();
  pid_t pid = fork();

  if (pid == 0)
    { 
      unlink (path); //Removing output file
      result = unsafe(i);
      FILE *fp;
      fp = fopen (path, "w");
      if (fp == NULL)
        { 
          //printf("Error in creating file\n");
          exit(0);
        }
        else 
          { 
            fwrite (&result,sizeof(int), 1, fp);
            fclose(fp);
          }
        exit(0);
    }


  if (pid > 0)
    {
      while(true)
        { 
          auto res = waitpid (pid, NULL, WNOHANG);
            if (res > 1)
            {
                       FILE *fp1;
              fp1 = fopen(path, "r");

              if(fp1 == NULL) {
                  //perror("Error opening file");
                  return-2;
                }

              else {
                //printf("reading file...   ");
                fread (&result, sizeof(int), 1, fp1);
                fclose(fp1);
                return result;
              }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            auto curr_time = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::microseconds>
            (curr_time - start_time).count() / 1000000.0;
              
              if( elapsed > 1) 
                { 
                  return -1;
                }
        }


    }

  return result;
}
