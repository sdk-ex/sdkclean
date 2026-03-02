/**/

#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sysexits.h>

// exit code: 0
#define STATUS_OK  EX_OK
// exit code: 70 (internal software error)
#define STATUS_ERR EX_SOFTWARE 

// exit code 1
// #define STATUS_ERR (CMD_FAIL, within commands.h ?)

#endif

