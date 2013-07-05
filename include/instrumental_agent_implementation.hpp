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

    inline InstrumentalStatus Agent::increment(const char* metric, double value, time_t time, int count)
    {
        return agent_add_data_point(_agent, INSTRUMENTAL_INCREMENT, metric, value, time, count);
    }

    inline InstrumentalStatus Agent::gauge(const char* metric, double value, time_t time, int count)
    {
        return agent_add_data_point(_agent, INSTRUMENTAL_GAUGE, metric, value, time, count);
    }

    inline InstrumentalStatus Agent::guage_absolute(const char* metric, double value, time_t time)
    {
        return agent_add_data_point(_agent, INSTRUMENTAL_GAUGE_ABSOLUTE, metric, value, time, 1);
    }
}