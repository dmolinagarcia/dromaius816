// log.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// log.h - Johan Smet - BSD-3-Clause (see LICENSE)

#ifndef DROMAIUS_LOG_H
#define DROMAIUS_LOG_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

// Get timestamp for logging
static inline void get_timestamp(char *buffer, size_t len) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    struct tm *t = localtime(&tv.tv_sec);

    snprintf(buffer, len, "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec,
             tv.tv_usec / 1000); 
}

// Global extern variable to store log status
extern int LOG_STATUS;

static inline void log_message(const char *file, const char *func, int line, const char *format, ...) {
    if (LOG_STATUS) {
        char timestamp[24];
        get_timestamp(timestamp, sizeof(timestamp));
        char log_buffer[512];
        va_list args;
        va_start(args, format);
        vsnprintf(log_buffer, sizeof(log_buffer), format, args);
        va_end(args);
        switch (LOG_STATUS) {
            case 1:
                printf("[ %s ] - %s\n", timestamp, log_buffer);
            break;
            case 2:
                printf("[ %s ] [ %s:%s:%d ] - %s\n", timestamp, file, func, line, log_buffer);
                break;    
        }
    }
}

// MACRO for printing log, if status is enabled
#define LOG(format, ...) log_message(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

// Enable and disable functions
void enable_log_state(int level);
void disable_log_state();

#ifdef __cplusplus
}
#endif

#endif // DROMAIUS_LOG_H

//> 
//> 
//> 
//> void log_printf(int64_t tick, const char *fmt, ...);
//> 
//> #ifdef DMS_LOG_TRACE
//> 	#ifdef LOG_SIMULATOR
//> 		#define LOG_TRACE(f, ...)	log_printf(LOG_SIMULATOR->current_tick, (f), __VA_ARGS__)
//> 	#else
//> 		#define LOG_TRACE(f, ...)	log_printf(-1, (f), __VA_ARGS__)
//> 	#endif
//> #else
//> 	#define LOG_TRACE(f, ...)
//> #endif
//> 
//> #ifdef __cplusplus
//> }
//> #endif
//> 

