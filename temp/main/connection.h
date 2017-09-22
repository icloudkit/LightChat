/*
 * Copyright (C) 2015 The CloudKit Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// 2017/5/31.
//

#ifndef LIGHTCHAT_CONNECTION_H
#define LIGHTCHAT_CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>

#include "transport.h"
#include "time_unit.h"

/* 定义端口号 */
#define SERV_PORT 4567
/* 最大连接数 */
#define MAX_CONN  10

/*
typedef struct listening_s listening_t;

struct listening_s {

};
*/

/*
typedef struct configuration_s configuration_t;

struct configuration_s {

};
*/

typedef enum {
    SUCCESS,
    WAIT_FOR_EVENT,
    FALLBACK,
    CLOSE,
    FATAL_ERROR
} result;

int get_transport(int argc, char *argv[]);

/**
 * Is {@link connection} open and ready.
 * Returns <tt>true</tt>, if connection is open and ready, or <tt>false</tt> otherwise.
 *
 * @return <tt>true</tt>, if connection is open and ready, or <tt>false</tt> otherwise.
 */
bool is_open();

/**
 * Sets the {@link Connection} mode.
 *
 * @param is_blocking the {@link connection} mode. <tt>true</tt>, if {@link connection} should operate in blocking mode,
 * or <tt>false</tt> otherwise.
 */
void configure_blocking(bool is_blocking);

/**
 * @return the {@link connection} mode.
 * <tt>true</tt>, if {@link connection} is operating in blocking mode, or <tt>false</tt> otherwise.
 */
bool is_blocking();

/**
 * Get the connection peer address
 * @return the connection peer address
 */
struct sockaddr_in get_peer_address();

/**
 * Get the connection local address
 * @return the connection local address
 */
struct sockaddr_in get_local_address();

/**
 * Get the default size of buffers, which will be allocated for reading data from {@link connection}.
 * The value less or equal to zero will be ignored.
 *
 * @return the default size of buffers, which will be allocated for reading data from {@link connection}.
 */
int get_read_buffer_size();

/**
 * Set the default size of buffers, which will be allocated for * reading data from {@link connection}.
 * The value less or equal to zero will be ignored.
 *
 * @param read_buffer_size the default size of buffers, which will be allocated for reading data
 * from {@link connection}.
 */
void set_read_buffer_size(int read_buffer_size);

/**
  * Get the default size of buffers, which will be allocated for writing data to {@link connection}.
  *
  * @return the default size of buffers, which will be allocated for writing data to {@link connection}.
 */
int get_write_buffer_size();

/**
 * Set the default size of buffers, which will be allocated for writing data to {@link connection}.
 *
 * @param write_buffer_size the default size of buffers, which will be allocated for writing data
 * to {@link connection}.
 */
void set_write_buffer_size(int write_buffer_size);

/**
 * Get the max size (in bytes) of asynchronous write queue associated with connection.
 *
 * @return the max size (in bytes) of asynchronous write queue associated with connection.
 */
int get_max_async_write_queue_size();

/**
 * Set the max size (in bytes) of asynchronous write queue associated with connection.
 *
 * @param maxAsyncWriteQueueSize the max size (in bytes) of asynchronous write queue associated with connection.
 */
void set_max_async_write_queue_size(int max_async_write_queue_size);

/**
 * Returns the current value for the blocking read timeout converted to the
 * provided {@link time_unit} specification.  If this value hasn't been
 * explicitly set, it will default to {@value #DEFAULT_READ_TIMEOUT} seconds.
 *
 * @param unit the {@link time_unit} to convert the returned result to.
 */
long get_read_timeout(time_unit unit);

/**
* Specifies the timeout for the blocking reads.  This may be overridden on
* a per-connection basis.
* A value of zero or less effectively disables the timeout.
*
* @param timeout the new timeout value
* @param unit the {@link time_unit} specification of the provided value.
*
* @see Connection#setReadTimeout(long, java.util.concurrent.TimeUnit)
*/
void set_read_timeout(long timeout, time_unit unit);

/**
 * Returns the current value for the blocking write timeout converted to the
 * provided {@link TimeUnit} specification.  If this value hasn't been
 * explicitly set, it will default to {@value #DEFAULT_WRITE_TIMEOUT} seconds.
 *
 * @param unit the {@link time_unit} to convert the returned result to.
 */
long get_write_timeout(time_unit unit);

/**
 * Specifies the timeout for the blocking writes.  This may be overridden on
 * a per-connection basis.
 * A value of zero or less effectively disables the timeout.
 *
 * @param timeout  the new timeout value
 * @param unit the {@link time_unit} specification of the provided value.
 */
void set_write_timeout(long timeout, time_unit unit);

/**
 * Close the {@link connection} silently, no notification required on completion or failure.
 */
void terminateSilently();

/**
 * Close the {@link connection}
 *
 * @return {@link Future}, which could be checked in case, if close operation will be run asynchronously
 */
void terminate();

/**
 * Gracefully close the {@link connection}
 *
 * @return {@link Future}, which could be checked in case, if close operation will be run asynchronously
 */
void close_handle();

/**
 * Gracefully close the connection silently, no notification required on completion or failure.
 */
void close_silently();



// static void va_overload(int p1, int p2, ...);
// static void va_overload_s(int p1, int p2);

// CloseType LOCALLY REMOTELY

/**
  * Checks if this <tt>Connection</tt> is open and ready to be used.
  * If this <tt>Connection</tt> is closed - this method throws
  * {@link IOException} giving the reason why this <tt>Connection</tt> was closed.
  *
  * @throws IOException
 */





// -------------------------------------------------------------------------------------------------------------------//

void *listening(configuration_t *cfg);

typedef void(*completion_handler)(char *);
// void get_callBack(completion_handler callback);
// void callback(char *s);
/**
 * Gracefully close the connection
 *
 * @param ch_callback completion_handler to be called, when the connection is closed.
 * @deprecated
 */
// void close_handle(completion_handler ch_callback);

// void assert_open();
// get_close_reason
// deprecated configure_standalone
// deprecated is_standalone

// obtain_processor get_processor set_processor
// get_processor_selector set_processor_selector

// obtain_processor_state
// execute_in_event_thread
// get_memory_manager


// simulate_io_event
// enable_io_event
// disable_io_event

// get_monitoring_config
// terminate_with_reason

// close_with_reason
// add_close_listener
// remove_close_listener
// notify_connection_error

#endif //LIGHTCHAT_CONNECTION_H