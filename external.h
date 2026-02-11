#ifndef EXECUTE_H
#define EXECUTE_H

#include <sys/types.h>

pid_t execute_external(char **args, int background);

#endif