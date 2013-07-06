#include "instrumental_agent.hpp"

#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[])
{
    if (argc < 5) {
        cerr << "Usage: cli_tool api_key type metric value" << endl
            << "Where:" << endl
            << "\tapi_key\tis the API_KEY from instrumentalapp.com" << endl
            << "\ttype\tis one of inc, gauge, absolute" << endl
            << "\tmetric\tis the metric name to record" << endl
            << "\tvalue\tis the floating point value to record" << endl;
    }

    InstrumentalStatus status;
    Instrumental::Agent agent(argv[1]);

    status = agent.connect();
    if (status != INSTRUMENTAL_OK) {
        cerr << "Could not connect to collector: " <<  status << endl;
        return 1;
    }

    std::string type = argv[2];
    const char* metric = argv[3];
    const char* value_string = argv[4];
    float value = atof(value_string);

    if (type == "inc" || type == "increment") {
        agent.increment(metric, value);
    } else if (type == "gauge") {
        agent.gauge(metric, value);
    } else if (type == "gauge_absolute" || type == "abs" || type == "absolute") {
        agent.absolute(metric, value);
    } else {
        cerr << "Unknown type: " << type << endl;
        return 1;
    }
    return 0;
}