#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>

extern "C" {
#include "ex1.h"
}

std::string captureOutput(void (*func)(int), int arg) {
  int pipefd[2];
  char buffer[128];
  std::string result;

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    func(arg);
    exit(EXIT_SUCCESS);
  } else {
    close(pipefd[1]);

    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
      buffer[n] = '\0';
      result += buffer;
    }
    close(pipefd[0]);

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
