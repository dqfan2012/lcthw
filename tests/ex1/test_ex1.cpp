#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

extern "C" {
#include "ex1.h"
}

// Function to capture output of a given function
std::string captureOutput(void (*func)(int), int arg) {
  int pipefd[2];
  char buffer[128];
  std::string result;

  // Create a pipe
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  // Fork the process
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {  // Child process
    // Redirect stdout to the pipe
    close(pipefd[0]);  // Close unused read end
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    // Call the function
    func(arg);
    exit(EXIT_SUCCESS);
  } else {  // Parent process
    // Close write end of the pipe
    close(pipefd[1]);

    // Read the output from the pipe
    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
      buffer[n] = '\0';
      result += buffer;
    }
    close(pipefd[0]);

    // Wait for the child process to finish
    wait(NULL);
  }

  return result;
}

TEST(Ex1Test, OutputTest) {
  std::string expected_output = "You are 100 miles away.\n";
  std::string actual_output = captureOutput(print_distance, 100);
  EXPECT_EQ(expected_output, actual_output);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
