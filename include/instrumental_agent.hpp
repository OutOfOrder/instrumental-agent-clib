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
#ifndef INSTRUMENTAL_AGENT_HPP
#define INSTRUMENTAL_AGENT_HPP

/**
 * \file instrumental_agent.hpp
 *
 * Public C++ API for the Instrumental Agent
 */

#ifndef __cplusplus
#error This header only supports C++
#endif

#include "instrumental_agent.h"

namespace Instrumental {
    /**
     * A instrumental agent class wrapper
     */
    class Agent {
    public:
        /**
         * Create a new Agent against a custom collector endpoint
         *
         * \param api_key       The api_key from instrumentalapp.com
         * \param collector     The collector endpoint.. e.g. myendpoint:8000. defaults to ::INSTRUMENTAL_ENDPOINT
         */
        Agent(const char* api_key, const char* collector = INSTRUMENTAL_ENDPOINT);
        ~Agent();

        /**
         * Get if the agent is enabled
         *
         * \return true if enabled
         */
        bool isEnabled();
        /**
         * Set the enabled state
         *
         * \param enabled   true to enable, false to disable
         */
        void enable(bool enabled);

        /**
         * Get if the agent is connected
         *
         * \return true if connected
         */
        bool isConnected();
        /**
         * Connect the agent
         *
         * \return an ::InstrumentalStatus value.
         */
        InstrumentalStatus connect();

        /**
         * Disconnect the agent
         *
         * \return an ::InstrumentalStatus value.
         */
        InstrumentalStatus disconnect();

        /**
         * Increments a data point
         */
        InstrumentalStatus increment(const char* metric, double value = 1, time_t time = time_now(), int count = 1);

        /**
         * Sets a gauge data point
         */
        InstrumentalStatus gauge(const char* metric, double value, time_t time = time_now(), int count = 1);

        /**
         * Sets an absolute gauge data point
         */
        InstrumentalStatus absolute(const char* metric, double value, time_t time = time_now());
    public:
        /**
         * returns the time for "now"
         */
        static time_t time_now();
    private:
        // disable copying
        Agent(const Agent&);
        Agent& operator=(const Agent&);
    private:
        InstrumentalAgent * _agent;
    };
}

#include "instrumental_agent_implementation.hpp"

#endif // INSTRUMENTAL_AGENT_HPP