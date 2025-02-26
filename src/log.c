// log.c - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// log.c - Johan Smet - BSD-3-Clause (see LICENSE)

#include "log.h"

// Default status is disabled
int LOG_STATUS = 0;
int LOG_DETAIL = 1;

// Enable log
void set_log_state(int level, int detail)  { LOG_STATUS = level; LOG_DETAIL=detail;}

