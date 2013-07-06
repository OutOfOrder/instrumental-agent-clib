/*
 InstrumentalAgent-clib

 Copyright (c) 2013 Edward Rudd

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

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
