CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(IntrumentalAgent)

find_package(CURL REQUIRED)

include_directories(
    include
    ${CURL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

SET(PUBLIC_HEADERS
    include/instrumental_agent.h
    include/instrumental_agent.hpp
    include/instrumental_agent_implementation.hpp
)
source_group("Interface" FILES ${PUBLIC_HEADERS})
SET(LIB_SOURCE
    src/agent.h
    src/agent.c
    src/agent_curl.c
)
source_group("Library" FILES ${LIB_SOURCE})

ADD_LIBRARY(InstrumentalAgent
    ${PUBLIC_HEADERS}
    ${LIB_HEADERS}
    ${LIB_SOURCE}
)
SET_TARGET_PROPERTIES(InstrumentalAgent
    PROPERTIES
        COMPILE_DEFINITIONS_DEBUG "DEBUG"
)
TARGET_LINK_LIBRARIES(InstrumentalAgent
    ${CURL_LIBRARY}
)

ADD_EXECUTABLE(cli_tool
    tests/cli_tool.cc
)
TARGET_LINK_LIBRARIES(cli_tool
    InstrumentalAgent
)