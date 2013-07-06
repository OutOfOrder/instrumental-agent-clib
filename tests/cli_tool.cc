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