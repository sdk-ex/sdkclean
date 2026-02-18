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

#define STATUS_OK  EX_OK         // 0
#define STATUS_ERR EX_SOFTWARE   // 70 (Internal software error) 
                                 // OR use EXIT_FAILURE (1) if you prefer generic

#endif

