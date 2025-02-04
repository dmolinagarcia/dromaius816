// DEBUG
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

#define DEBUG

// Función para obtener el timestamp actual
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

// Macro de depuración mejorado con timestamp
#ifdef DEBUG
    #define DBG_PRINT(fmt, ...) \
        printf("[%s] [DEBUG] %s:%d:%s(): " fmt "\n", get_timestamp().c_str(), __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define DBG_PRINT(fmt, ...)
#endif