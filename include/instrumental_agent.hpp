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