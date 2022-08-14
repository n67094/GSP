#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#ifdef NDEBUG
#  include <seven/base/types.h>
#  include <stdarg.h>
#  include <stdbool.h>

#  define MGBA_REG_DEBUG_ENABLE (volatile u16 *)0x4FFF780
#  define MGBA_REG_DEBUG_FLAGS (volatile u16 *)0x4FFF700

#  define MGBA_REG_DEBUG_STRING (char *)0x4FFF600
#  define LOG_INIT() LogInit()
#  define LOG_CLOSE() LogClose()

#  define LOG_ASSERT(msg, ...) LogAssert(msg, ##__VA_ARGS__)
#  define LOG_INFO(msg, ...) LogInfo(msg, ##__VA_ARGS__)
#  define LOG_DEBUG(msg, ...) LogDebug(msg, ##__VA_ARGS__)
#  define LOG_WARNING(msg, ...) LogWarning(msg, ##__VA_ARGS__)
#  define LOG_ERROR(msg, ...) LogError(msg, ##__VA_ARGS__)
#  define LOG_FATAL(msg, ...) LogFatal(msg, ##__VA_ARGS__)

bool LogInit();
void LogClose();

void LogEx(int type, const char *msg, va_list args);

void LogAssert(const char *msg, ...);
void LogInfo(const char *msg, ...);
void LogDebug(const char *msg, ...);
void LogWarning(const char *msg, ...);
void LogError(const char *msg, ...);
void LogFatal(const char *msg, ...);

#else // Not NDEBUG
#  define LOG_INIT(...)
#  define LOG_CLOSE(...)

#  define LOG_ASSERT(msg, ...)
#  define LOG_INFO(msg, ...)
#  define LOG_DEBUG(msg, ...)
#  define LOG_WARNING(msg, ...)
#  define LOG_ERROR(msg, ...)
#  define LOG_FATAL(msg, ...)

#endif // NDEBUG
#endif // UTILS_LOG
