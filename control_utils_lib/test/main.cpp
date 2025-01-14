// test/main.cpp
#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(pid_test);
IMPORT_TEST_GROUP(clamp_test);

int main(int ac, char **av) { return CommandLineTestRunner::RunAllTests(ac, av); }