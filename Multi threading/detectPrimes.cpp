#include "detectPrimes.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <atomic>

uint64_t counter = 0;
std::atomic<bool>  global_finished (false);
int64_t next_num;
int64_t max_num;
int64_t min_num;
int64_t divisor_size;
std::vector<int> result_temp;
std::atomic_bool kill_signal{false};

//simple_barrier class was provided in lecture notes
//------------------------------------------------------------------------//
class simple_barrier {
  std::mutex m_;
  std::condition_variable cv_;
  int n_remaining_, count_;
  bool coin_;
public:
  simple_barrier(int count) {
    count_ = count;
    n_remaining_ = count_;
    coin_ = false;
  }
  bool wait() {
    std::unique_lock<std::mutex> lk(m_);
    if( n_remaining_ == 1) {
      coin_ = ! coin_;
      n_remaining_ = count_;
      cv_.notify_all();
      return true;
    }
    auto old_coin = coin_;
    n_remaining_ --;
    cv_.wait(lk, [&](){ return old_coin != coin_; });
    return false;
  }
};

//---------------------------------------------------------------------------//

static bool primes(int64_t n, int64_t min, int64_t max)
{
    // handle trivial cases
  if (n < 2) return false;
  if (n <= 3) return true; // 2 and 3 are primes
  if (n % 2 == 0)         // handle multiples of 2
  {
    kill_signal.store(true);
    return false;
    } 

   if (n % 3 == 0 )       // handle multiples of 3
  {
    kill_signal.store(true);
    return false;
    } 
  // try to divide n by every number 5 .. sqrt(n)
  int64_t temp = min/6;
  if(temp == 0){
    temp= 1;
  }

  int64_t i = temp * 6 - 1;
  while (i <= max) {
    if (n % i == 0) return false;
    if (n % (i + 2) == 0) return false;
    i += 6;
  if(kill_signal.load()){
    return false;
    }
  }
  // didn't find any divisors, so it must be a prime
  return true;
}

// returns true if n is prime, otherwise returns false
// -----------------------------------------------------------------------------
// to get full credit for this assignment, you will need to adjust or even
// re-write the code in this function to make it multithreaded.
static void is_prime(const std::vector<int64_t> nums, int tid, simple_barrier &barrier, std::vector<int64_t> & result, int n_threads)
{
  while(true){
    if(barrier.wait()){
      if(counter < nums.size()){
        next_num = nums.at(counter);
        counter++;
        max_num = sqrt(next_num);
        divisor_size= max_num/n_threads+1;
        min_num = 1;
        }
      else {
        global_finished.store(true, std::memory_order_relaxed);
      }
    }

    barrier.wait(); //parallel work
     if(global_finished.load(std::memory_order_relaxed)){
        break;
     }
     else{
       int64_t temp= divisor_size*tid;
       int64_t max_check = (divisor_size)*(tid+1); 
       int64_t temp2;
       if(max_check>max_num) {
         temp2=max_num;}

        else{
          temp2=max_check;
        }

       if (primes(next_num, temp, temp2)) {
         result_temp.at(tid) = 1;
       }
     }

      if(barrier.wait()){
        bool check_prime=true;
        for (int i=0; i<n_threads; i++){
          if(result_temp.at(i)==0){
            check_prime= 0;
          }
        }
        if(check_prime){
          result.push_back(next_num);
        }

        for (int i=0; i<n_threads; i++)
      {
      result_temp.at(i)=0;       //Resetting global temp result vector with 0
      }
      kill_signal.store(false);
  }
      barrier.wait();
  }
}


// This function takes a list of numbers in nums[] and returns only numbers that
// are primes.
//
// The parameter n_threads indicates how many threads should be created to speed
// up the computation.
// -----------------------------------------------------------------------------
// You will most likely need to re-implement this function entirely.
// Note that the current implementation ignores n_threads. Your multithreaded
// implementation must use this parameter.
std::vector<int64_t>
detect_primes(const std::vector<int64_t> & nums, int n_threads)
{
  std::vector<int64_t> result;
  simple_barrier barrier(n_threads);
  std::vector<std::thread> threads;

      for (int i=0; i<n_threads; i++)
      {
      result_temp.push_back(0);       //filling global temp result vector with 0
      }

    for (auto i =0; i<n_threads; i++){
      threads.emplace_back(is_prime, nums, i, std::ref(barrier), std::ref(result), n_threads);
    }

    for (auto && t : threads)
    t.join();
    
  return result;
}
