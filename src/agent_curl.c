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

#include "agent.h"

#include "curl/curl.h"

#include <stdlib.h>
#include <string.h>

InstrumentalStatus agent_connect_internal(InstrumentalAgent * agent)
{
    CURL * handle;
    CURLcode res;

    if (agent->connection != 0) return INSTRUMENTAL_OK;

    handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, agent->collector);
    curl_easy_setopt(handle, CURLOPT_CONNECT_ONLY, 1L);

    res = curl_easy_perform(handle);
    if (res != CURLE_OK) {
        curl_easy_cleanup(handle);
        return INSTRUMENTAL_CONNECT_ERROR;
    }

    agent->connection = handle;

    return INSTRUMENTAL_OK;
}

InstrumentalStatus agent_disconnect_internal(InstrumentalAgent * agent)
{
    CURL * handle;

    if (agent->connection == 0) return INSTRUMENTAL_NOT_CONNECTED;
    handle  = (CURL*)agent->connection;

    curl_easy_cleanup(handle);

    agent->connection = 0;

    return INSTRUMENTAL_OK;
}

/**
 * internal method to wait on the curl network socket
 */
static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
    struct timeval tv;
    fd_set infd, outfd, errfd;
    int res;

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec= (timeout_ms % 1000) * 1000;

    FD_ZERO(&infd);
    FD_ZERO(&outfd);
    FD_ZERO(&errfd);

    FD_SET(sockfd, &errfd); /* always check for error */

    if(for_recv)
    {
        FD_SET(sockfd, &infd);
    }
    else
    {
        FD_SET(sockfd, &outfd);
    }

    /* select() returns the number of signalled sockets or -1 */
    res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
    return res;
}

InstrumentalStatus agent_send_data_internal(InstrumentalAgent * agent, const char * data)
{
    CURL * handle;
    CURLcode ret;
    long sockextr;
    size_t iolen;
    curl_socket_t sockfd;

    if (agent->connection == 0) return INSTRUMENTAL_NOT_CONNECTED;
    handle  = (CURL*)agent->connection;

    ret = curl_easy_getinfo(handle, CURLINFO_LASTSOCKET, &sockextr);
    if (ret != CURLE_OK || sockextr == -1) {
        return INSTRUMENTAL_NOT_CONNECTED;
    }
    sockfd = sockextr;

    if (!wait_on_socket(sockfd, 0, 5000L))
    {
        return INSTRUMENTAL_TIMEOUT;
    }

    ret = curl_easy_send(handle, data, strlen(data), &iolen);
    if (ret != CURLE_OK) {
        if (ret == CURLE_AGAIN) {
            return INSTRUMENTAL_SOCKET_NOT_READY;
        } else {
            return INSTRUMENTAL_SOCKET_ERROR;
        }
    }

    return INSTRUMENTAL_OK;
}

InstrumentalStatus agent_receive_data(InstrumentalAgent * agent, char * buff, size_t bufflen)
{
    CURL * handle;
    CURLcode ret;
    long sockextr;
    size_t iolen;
    curl_socket_t sockfd;

    if (agent->connection == 0) return INSTRUMENTAL_NOT_CONNECTED;
    handle  = (CURL*)agent->connection;

    ret = curl_easy_getinfo(handle, CURLINFO_LASTSOCKET, &sockextr);
    if (ret != CURLE_OK || sockextr == -1) {
        return INSTRUMENTAL_NOT_CONNECTED;
    }
    sockfd = sockextr;

    if (!wait_on_socket(sockfd, 1, 5000L))
    {
        return INSTRUMENTAL_TIMEOUT;
    }

    ret = curl_easy_recv(handle, buff, bufflen, &iolen);
    if (ret != CURLE_OK)
    {
        if (ret == CURLE_AGAIN) {
            return INSTRUMENTAL_SOCKET_NOT_READY;
        } else {
            return INSTRUMENTAL_SOCKET_ERROR;
        }
    }

    // terminate the data
    if (iolen >= bufflen) iolen = bufflen - 1;

    buff[iolen] = '\0';

    return INSTRUMENTAL_OK;
}