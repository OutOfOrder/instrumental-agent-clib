Instrumental Agent clib
-----------------------

A simple C/C++ API to push data to http://instrumentalapp.com/

Requirements
============
This project currently requires [libcurl](http://curl.haxx.se/libcurl/)
for network connectivity. The code, however, is written in a way where
other network backend could be added. All of the curl related network code is
isolated and kept in one file agent_curl.c.

Compilation
===========
This project uses [CMake] (http://cmake.org/) for the build system.
Simply download cmake for your platform and have it generate the project
files of your choice (XCode, make, Visual Studio, etc..)

*NOTE* This has only been tested on Mac OS X and Linux currently.

Usage
=====

This is a C library with a simple C++ header-only wrapper class over the C library.
So, simply include C or C++ header from the include directory and link to the 
libInstrumentalAgent.a library.

C++ Interface
=============
The C++ interface is simply as

> Instrumental::Agent agent("my_api_key");
> agent.connect();
> agent.increment("test.inc_val");
> agent.gauge("test.gauge", 34.55);
> agent.disconnect();

Testing
=======
There aren't any tests currently only a sample cli_tool that gets built with the
library that allows simple setting of values.

TODO / Future
=============

Items that I'll be working on in the near future

* a background thread/queue so your application can push data to a worker thread
* a proper test suite to validate the C library
* add in some code to handle re-connect
