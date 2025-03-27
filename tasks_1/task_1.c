#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

// Function to keep the process alive
void wait_for_enter(const char* process_name){
  printf("Press enter to exit %s...\n", process_name);
  getchar();
  exit(EXIT_SUCCESS);
}

// Describe the process
void process_describe(const char* process_name, const char* parent_name, bool exit_flag) {
  printf("Creating process %s (PID = %d), child of %s (PID = %d)\n",
         process_name, getpid(), parent_name, getppid());

  // The flag keeps the process alive
  if (exit_flag) {
    wait_for_enter(process_name);
  }
}

// Create the process
pid_t safe_fork() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Erro ao criar processo com fork");
    exit(EXIT_FAILURE);
  }

  return pid;
}

int main() {
  printf("P1 (PID = %d)\n", getpid());

  pid_t pid_2 = safe_fork();
  if (pid_2 == 0) {
    process_describe("P2", "P1", false);

    pid_t pid_4 = safe_fork();
    if (pid_4 == 0) {
      process_describe("P4", "P2", true);
    }

    pid_t pid_5 = safe_fork();
    if (pid_5 == 0) {
      process_describe("P5", "P2", true);
    }

    wait_for_enter("P2");
  }

  pid_t pid_3 = safe_fork();
  if (pid_3 == 0) {
    process_describe("P3", "P1", true);
  }

  wait_for_enter("P1");

  return 0;
}