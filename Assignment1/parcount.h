/* File: parcount.h
 * Author: Nicholas Graham
 * Date: January 29, 2017
 * version: 1.0
 * Description: Assignment One: look ate different methods of syncronization
 * using C++ built in threads.
 */

#ifndef __parcount_include__
#define __parcount_include__

// Includes
#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <ctime>
#include <ratio>
#include <chrono>

#define time std::chrono::high_resolution_clock::time_point
#define now(void) std::chrono::high_resolution_clock::now()
#define time_delta std::chrono::duration<double>
#define time_duration(s, e) std::chrono::duration_cast<std::chrono::duration<double>>(e - s)

// Function Definitions
void stageOne (int *counter, int number);
void stageTwo (int *counter, int number);
void stageThree (int *counter, int number);
void stageFour (std::atomic_int *counter, int number);
void stageFive (int *counter, int number);
void tears (int *i);

#endif
