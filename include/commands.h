/**/

#ifndef COMMANDS_H
#define COMMANDS_H

#include "pch.h"

// re-established consts to ensure consistency, may be deleted later
#define CMD_SUCCESS 0
#define CMD_FAIL 1

int run_apt(const char* args, void* context);
int run_trash(const char* path, void* context);
int run_journal_maint(const char* args, void* context);

#endif







