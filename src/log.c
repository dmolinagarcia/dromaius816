// log.c - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// log.c - Johan Smet - BSD-3-Clause (see LICENSE)

#include "log.h"

// Default status is disabled
int LOG_STATUS = false;

// Enable log
void enable_log_state(int level)  { LOG_STATUS = level; }
void disable_log_state() { LOG_STATUS = 0; }

//> #include <stdarg.h>
//> #include <stdio.h>
//> #include <inttypes.h>
//> 
//> void log_printf(int64_t tick, const char *fmt, ...) {
//> 	va_list args;
//> 
//> 	printf("%"PRId64": ", tick);
//> 
//> 	va_start(args, fmt);
//>     vprintf(fmt, args);
//>     va_end(args);
//> 
//> 	printf("\n");
//> }

/*

//> DEBUG:h // gui/debug.h - Daniel MOlina - BSD-3-Clause (see LICENSE)
//> DEBUG:h // Debugging functions for development
//> DEBUG:h 
//> DEBUG:h // DEBUG
//> DEBUG:h #define DEBUG
//> DEBUG:h 
//> DEBUG:h 
//> DEBUG:h // DEBUG Macro
//> DEBUG:h #ifdef DEBUG
//> DEBUG:h     #define DBG_PRINT(fmt, ...) \
//> DEBUG:h         printf("[%s] [DEBUG] %s:%d:%s(): " fmt "\n", get_timestamp().c_str(), __FILE__, __LINE__, __func__, ##__VA_ARGS__)
//> DEBUG:h #else
//> DEBUG:h     #define DBG_PRINT(fmt, ...)
//> DEBUG:h #endif


#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

// Get Current Timestamp
inline std::string get_timestamp() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    auto now_ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::tm now_tm = *std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << now_ms.count();
    return oss.str();
}


*/