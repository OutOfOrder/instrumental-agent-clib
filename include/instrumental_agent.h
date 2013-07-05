#ifndef INSTRUMENTAL_AGENT_H
#define INSTRUMENTAL_AGENT_H

/**
 * \file instrumental_agent.h
 *
 * Public C API for the Instrumental Agent
 */

/* needed for cross platform time_t definition */
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif
    /** The default instrumental endpoint */
    #define INSTRUMENTAL_ENDPOINT "collector.instrumentalapp.com:8000"

    /** The opaque object for the agent */
    typedef struct InstrumentalAgent InstrumentalAgent;

    /** Status enums */
    typedef enum {
        INSTRUMENTAL_OK,                /**< The function completed successfully */
        INSTRUMENTAL_QUEUE_FULL,
        INSTRUMENTAL_NOT_CONNECTED,
        INSTRUMENTAL_FAILED
    } InstrumentalStatus;

    /** Metric types */
    typedef enum {
        INSTRUMENTAL_INCREMENT,         /**< an incrementing metric.. e.g signups */
        INSTRUMENTAL_GAUGE,             /**< a guage value.. most everything else */
        INSTRUMENTAL_GAUGE_ABSOLUTE,    /**< a pre-calculated absolute metric.. used if your application aggregates data itself */
    } InstrumentalMetric;

    /**
     * Create a new agent
     *
     * \param api_key       the API key for instrumentalapp.com
     * \param collector     the collector endpoint to connect to e.g. myhost:8000.
     *                      Pass ::INSTRUMENTAL_ENDPOINT to use the default endpoint
     *
     * \return  The instrumental agent object
     */
    InstrumentalAgent * agent_create(const char * api_key, const char * collector);

    /**
     * Destroy an existing agent
     */
    void agent_destroy(InstrumentalAgent *);

    /**
     * Connect the agent
     */
    InstrumentalStatus agent_connect(InstrumentalAgent *);

    /**
     * Check if the agent is connected
     *
     * \return  Returns 1 if connected 0 otherwise
     */
    int agent_is_connected(InstrumentalAgent *);

    /**
     * Enable/Disable an agent
     *
     * \param enabled    0 for disabled, anything else to enable
     */
    void agent_set_enabled(InstrumentalAgent *, int enabled);

    /**
     * Get the enabled status of the agent
     *
     * \return  returns 0 for disabled, 1 for enabled
     */
    int agent_is_enabled(InstrumentalAgent *);

    /**
     * add a gauge datapoint
     *
     * \param type          the metric type
     * \param metric        the metric name to record
     * \param value         the value to record
     * \param when          the unix timestamp to record the data at
     * \param count         the # of times to record the value (server size multiplier). use 1 in most cases
     *
     * \return a ::InstrumentalStatus status
     *
     * \note When the agent is disabled this method will return queue no data ::INSTRUMENTAL_OK
     * \see agent_set_enabled
     */
    InstrumentalStatus agent_add_data_point(InstrumentalAgent *, InstrumentalMetric type,
                                            const char * metric, double value,
                                            time_t when, int count);

#ifdef __cplusplus
}
#endif

#endif // INSTRUMENTAL_AGENT_H
