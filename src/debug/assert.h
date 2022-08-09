#ifndef UTILS_ASSERT
#define UTILS_ASSERT

#ifdef NDEBUG
#  include "log.h"

#  define ASSERT(test)                                                                                                 \
    do {                                                                                                               \
      if (!(test))                                                                                                     \
        LOG_ASSERT("file: %s line: %d - %s\n", __FILE__, __LINE__, #test);                                             \
    } while (0)
#else
#  define ASSERT(test)

#endif // NDEBUG
#endif // UTILS_ASSERT
