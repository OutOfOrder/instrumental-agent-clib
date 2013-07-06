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

#include "agent.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

InstrumentalAgent * agent_create(const char* api_key, const char * collector)
{
    InstrumentalAgent * agent = malloc(sizeof(InstrumentalAgent));
    DEBUG_printf("Allocationg agent: %p of size: %lu\n", agent, sizeof(InstrumentalAgent));
    agent->flags = INTERNAL_FLAGS_ENABLED;
    agent->api_key = strdup(api_key);
    agent->collector = strdup(collector);
    agent->connection = 0;
    return agent;
}

void agent_destroy(InstrumentalAgent * agent)
{
    agent_disconnect(agent);
    free(agent->api_key);
    free(agent->collector);
    DEBUG_printf("Deallocating agent: %p\n", agent);
    free(agent);
}

InstrumentalStatus agent_connect(InstrumentalAgent * agent)
{
    DEBUG_printf("Connecting agent: %p\n", agent);
    InstrumentalStatus status = agent_connect_internal(agent);
    if (status != INSTRUMENTAL_OK) return status;
    return agent_authenticate(agent);
}

InstrumentalStatus agent_disconnect(InstrumentalAgent * agent)
{
    DEBUG_printf("Disconnecting agent: %p\n", agent);
    return agent_disconnect_internal(agent);
}

int agent_is_connected(InstrumentalAgent * agent)
{
    return (agent->connection == 0) ? 0 : 1;
}

void agent_set_enabled(InstrumentalAgent * agent, int enabled)
{
    if (enabled == 0) {
        agent->flags &= ~(INTERNAL_FLAGS_ENABLED);
    } else {
        agent->flags |= INTERNAL_FLAGS_ENABLED;
    }
}

int agent_is_enabled(InstrumentalAgent * agent)
{
    return (agent->flags & INTERNAL_FLAGS_ENABLED) > 0 ? 1 : 0;
}

InstrumentalStatus agent_add_data_point(InstrumentalAgent * agent, InstrumentalMetric type,
                                        const char *metric, double value,
                                        time_t when, int count)
{
    InstrumentalStatus status = INSTRUMENTAL_OK;

    switch (type) {
        case INSTRUMENTAL_INCREMENT:
            status = agent_send_data(agent, "increment %s %f %ld %d\n", metric, value, when, count);
            break;
        case INSTRUMENTAL_GAUGE:
            status = agent_send_data(agent, "gauge %s %f %ld %d\n", metric, value, when, count);
            break;
        case INSTRUMENTAL_GAUGE_ABSOLUTE:
            status = agent_send_data(agent, "gauge_absolute %s %f %ld\n", metric, value, when);
            break;
        default:
            status = INSTRUMENTAL_UNKNOWN;
            break;
    }

    return status;
}

InstrumentalStatus agent_authenticate(InstrumentalAgent * agent)
{
    InstrumentalStatus status;
    char buff[30];

    DEBUG_printf("Authenticating agent: %p\n", agent);

    status = agent_send_data(agent, "hello version %s agent clib\n", INSTRUMENTAL_AGENT_VERSION);
    if (status != INSTRUMENTAL_OK) return status;

    /** Eat the "ok" */
    status = agent_receive_data(agent, buff, 30);
    if (status != INSTRUMENTAL_OK) return status;

    status = agent_send_data(agent, "authenticate %s\n", agent->api_key);
    if (status != INSTRUMENTAL_OK) return status;

    /** Eat the "ok" */
    status = agent_receive_data(agent, buff, 30);
    if (status != INSTRUMENTAL_OK) return status;

    return status;
}

InstrumentalStatus agent_send_data(InstrumentalAgent * agent, const char * format, ...)
{
    InstrumentalStatus status = INSTRUMENTAL_OK;

    char *buff = 0;

    va_list args;
    va_start(args, format);

    int ret = vasprintf(&buff, format, args);
    if (ret != -1) {
        DEBUG_printf("Sending Data: %p :: %s\n", agent, buff);

        status = agent_send_data_internal(agent, buff);
        free(buff);
    } else {
        status = INSTRUMENTAL_MEMORY_ERROR;
    }

    va_end(args);

    return status;
}
