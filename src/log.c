// log.c - Johan Smet - BSD-3-Clause (see LICENSE)

#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>

void log_printf(int64_t tick, const char *fmt, ...) {
	va_list args;

	printf("%"PRId64": ", tick);

	va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

	printf("\n");
}

/*

//> DEBUG:h // gui/debug.h - Daniel MOlina - BSD-3-Clause (see LICENSE)
//> DEBUG:h // Debugging functions for development
//> DEBUG:h 
//> DEBUG:h // DEBUG
//> DEBUG:h #include <iostream>
//> DEBUG:h #include <chrono>
//> DEBUG:h #include <ctime>
//> DEBUG:h #include <iomanip>
//> DEBUG:h 
//> DEBUG:h #define DEBUG
//> DEBUG:h 
//> DEBUG:h // Get Current Timestamp
//> DEBUG:h inline std::string get_timestamp() {
//> DEBUG:h     using namespace std::chrono;
//> DEBUG:h 
//> DEBUG:h     auto now = system_clock::now();
//> DEBUG:h     auto now_time_t = system_clock::to_time_t(now);
//> DEBUG:h     auto now_ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
//> DEBUG:h 
//> DEBUG:h     std::tm now_tm = *std::localtime(&now_time_t);
//> DEBUG:h 
//> DEBUG:h     std::ostringstream oss;
//> DEBUG:h     oss << std::put_time(&now_tm, "%H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << now_ms.count();
//> DEBUG:h     return oss.str();
//> DEBUG:h }
//> DEBUG:h 
//> DEBUG:h // DEBUG Macro
//> DEBUG:h #ifdef DEBUG
//> DEBUG:h     #define DBG_PRINT(fmt, ...) \
//> DEBUG:h         printf("[%s] [DEBUG] %s:%d:%s(): " fmt "\n", get_timestamp().c_str(), __FILE__, __LINE__, __func__, ##__VA_ARGS__)
//> DEBUG:h #else
//> DEBUG:h     #define DBG_PRINT(fmt, ...)
//> DEBUG:h #endif

*/