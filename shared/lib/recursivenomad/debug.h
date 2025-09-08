/* SPDX-License-Identifier: MIT-0 OR CC0-1.0 *\
\*    Authored 2025 by Peter S. Hollander    */


#ifndef _RECURSIVENOMAD_DEBUG_H
#define _RECURSIVENOMAD_DEBUG_H



// Use hacky sizeof on multiple arguments to ensure a single integer output
// Final argument is the one evaluated, so append 0 to reduce compiler warnings
// Cast to a null pointer to guarantee false conditional evaluation
#define UNUSED(...)  ((void*)sizeof(__VA_ARGS__, 0))



#ifndef NDEBUG

    #ifndef DEBUG_OUTPUT_CHAR_LEN
        #define  DEBUG_OUTPUT_CHAR_LEN  80  // Assume classic terminal width
    #endif

    #ifndef WARN_PREFIX
        #define  WARN_PREFIX  "WARNING: "
    #endif

    #ifndef ERROR_PREFIX
        #define  ERROR_PREFIX  "! ERROR: "
    #endif

    #define DEBUG_TRACE(prefix, ...)                                         \
            do {                                                             \
                char trace[DEBUG_OUTPUT_CHAR_LEN];                           \
                int strLen = printf(prefix) + printf(__VA_ARGS__);           \
                int trcLen = sprintf(trace, "[%s:%i]", __FILE__, __LINE__);  \
                /* Overflow to the next line if too long */                  \
                if (strLen + trcLen >= DEBUG_OUTPUT_CHAR_LEN) {              \
                    printf("\n");                                            \
                    strLen = 0;                                              \
                }                                                            \
                /* Traceback right-justified */                              \
                printf("%*s", DEBUG_OUTPUT_CHAR_LEN - strLen, trace);        \
                printf("\n");                                                \
            } while (0);

    #define   DEBUG_LOG(...)  { printf(__VA_ARGS__); printf("\n"); }
    #define  DEBUG_WARN(...)  DEBUG_TRACE(WARN_PREFIX, __VA_ARGS__)
    #define DEBUG_ERROR(...)  DEBUG_TRACE(ERROR_PREFIX, __VA_ARGS__)

    #define DEBUG_UNEXPECTED_VALUE(value, format)                    \
                DEBUG_TRACE("Unexpected " #value ": ", format, value);

#else
    #define              DEBUG_LOG(...)  UNUSED(__VA_ARGS__);
    #define             DEBUG_WARN(...)  UNUSED(__VA_ARGS__);
    #define            DEBUG_ERROR(...)  UNUSED(__VA_ARGS__);
    #define DEBUG_UNEXPECTED_VALUE(...)  UNUSED(__VA_ARGS__);
#endif



#endif //_RECURSIVENOMAD_DEBUG_H
