#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
using namespace std;

// Function to keep the process alive
void wait_for_enter(string process_name){
  cout << "Press enter to exit " << process_name << endl;
  getchar();
  exit(EXIT_SUCCESS);
}

// Describe the process
void process_describe(string process_name, string parent_name, bool exit=true){
  cout << "Creating process " << process_name
      << " (PID = " << getpid() << "), child of "
      << parent_name << " (PID = " << getppid() << ")" << endl;

  // The flag keeps the process alive
  if (exit) wait_for_enter(process_name);
}

// Verify if the fork failed
pid_t safeFork(){
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }
  return pid;
}

int main() {
  cout << "P1 (PID = " << getpid() << ")" << endl;

  pid_t pid_2 = safeFork();
  if (pid_2 == 0) {
    process_describe("P2", "P1", false);

    pid_t pid_4 = safeFork();
    if (pid_4 == 0) process_describe("P4", "P2");

    pid_t pid_5 = safeFork();
    if (pid_5 == 0) process_describe("P5", "P2");

    wait_for_enter("P2");
  }

  pid_t pid_3 = safeFork();
  if (pid_3 == 0) process_describe("P3", "P1");

  wait_for_enter("P1");

  return 0;
}