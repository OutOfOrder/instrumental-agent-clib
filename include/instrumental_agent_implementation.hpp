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

/**
 * \file instrumental_agent_implementation.hpp
 *
 * Implementation file for the C++ wrapper.
 */
namespace Instrumental {
    inline Agent::Agent(const char* api_key, const char* collector)
    {
        _agent = agent_create(api_key, collector);
    }
    inline Agent::~Agent()
    {
        agent_destroy(_agent);
        _agent = 0;
    }

    inline bool Agent::isEnabled()
    {
        return agent_is_enabled(_agent) != 0;
    }

    inline void Agent::enable(bool enabled)
    {
        agent_set_enabled(_agent, enabled ? 1 : 0);
    }

    inline bool Agent::isConnected()
    {
        return agent_is_connected(_agent) != 0;
    }

    inline InstrumentalStatus Agent::connect()
    {
        return agent_connect(_agent);
    }
    inline InstrumentalStatus Agent::disconnect()
    {
        return agent_disconnect(_agent);
    }

    inline InstrumentalStatus Agent::increment(const char* metric, double value, time_t time, int count)
    {
        return agent_add_data_point(_agent, INSTRUMENTAL_INCREMENT, metric, value, time, count);
    }

    inline InstrumentalStatus Agent::gauge(const char* metric, double value, time_t time, int count)
    {
        return agent_add_data_point(_agent, INSTRUMENTAL_GAUGE, metric, value, time, count);
    }

    inline InstrumentalStatus Agent::absolute(const char* metric, double value, time_t time)
    {
        return agent_add_data_point(_agent, INSTRUMENTAL_GAUGE_ABSOLUTE, metric, value, time, 1);
    }

    inline time_t Agent::time_now()
    {
        return time(NULL);
    }
}