#ifdef NDEBUG

#  include <stdarg.h>
#  include <stdbool.h>

#  include "log.h"
#  include "printf.h"

bool LogInit()
{
  *MGBA_REG_DEBUG_ENABLE = 0xC0DE;
  return *MGBA_REG_DEBUG_ENABLE == 0x1DEA;
}

void LogClose() { *MGBA_REG_DEBUG_ENABLE = 0; }

void LogEx(int type, const char *msg, va_list args)
{
  int level;

  switch (type) {
  case 5: // ASSERT -> subset of ERROR
    level = 1 & 0x7;
    break;
  case 3: // INFO
    level = 3 & 0x7;
    break;
  case 4: // DEBUG
    level = 4 & 0x7;
    break;
  case 2: // WARNING
    level = 2 & 0x7;
    break;
  case 1: // ERROR
    level = 1 & 0x7;
    break;
  case 0: // FATAL:
    level = 0 & 0x7;
    break;
  default: // INFO
    level = 3 & 0x7;
  }

  vsnprintf(MGBA_REG_DEBUG_STRING, 0x100, msg, args);

  *MGBA_REG_DEBUG_FLAGS = level | 0x100;
}

void LogAssert(const char *msg, ...)
{
  va_list vlist;
  va_start(vlist, msg);

  LogEx(5, msg, vlist);

  va_end(vlist);
}

void LogInfo(const char *msg, ...)
{
  va_list vlist;
  va_start(vlist, msg);

  LogEx(3, msg, vlist);

  va_end(vlist);
}

void LogDebug(const char *msg, ...)
{
  va_list vlist;
  va_start(vlist, msg);

  LogEx(4, msg, vlist);

  va_end(vlist);
}

void LogWarning(const char *msg, ...)
{
  va_list vlist;
  va_start(vlist, msg);

  LogEx(2, msg, vlist);

  va_end(vlist);
}

void LogError(const char *msg, ...)
{
  va_list vlist;
  va_start(vlist, msg);

  LogEx(1, msg, vlist);

  va_end(vlist);
}

void LogFatal(const char *msg, ...)
{
  va_list vlist;
  va_start(vlist, msg);

  LogEx(0, msg, vlist);

  va_end(vlist);
}

#endif // NDEBUG
