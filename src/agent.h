#include "instrumental_agent.h"

#if DEBUG
    #define DEBUG_printf(...) fprintf(stdout, __VA_ARGS__);
#else
    #define DEBUG_printf(...)
#endif

#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
        #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
        #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
    #define DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define DLL_PUBLIC __attribute__ ((visibility ("default")))
        #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define DLL_PUBLIC
        #define DLL_LOCAL
    #endif
#endif

struct InstrumentalAgent
{
    void * connection;
    char * api_key;
    char * collector;
    int flags;
};

typedef enum {
    INTERNAL_FLAGS_ENABLED = 0x01,
} InternalFlags;

// internal methods
DLL_LOCAL InstrumentalStatus agent_authenticate(InstrumentalAgent *);
DLL_LOCAL InstrumentalStatus agent_send_data(InstrumentalAgent *, const char * format, ...)
    __attribute__ ((format (printf, 2, 3)));

// really internal methods defined in network library
DLL_LOCAL InstrumentalStatus agent_send_data_internal(InstrumentalAgent *, const char * data);
DLL_LOCAL InstrumentalStatus agent_receive_data(InstrumentalAgent *, char * buff, size_t bufflen);
DLL_LOCAL InstrumentalStatus agent_connect_internal(InstrumentalAgent *);
DLL_LOCAL InstrumentalStatus agent_disconnect_internal(InstrumentalAgent *);
