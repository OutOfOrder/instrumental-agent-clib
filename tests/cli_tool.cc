#include "instrumental_agent.hpp"

#include <iostream>

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

    return 0;
}