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

#ifndef LIGHTCHAT_TRANSPORT_H
#define LIGHTCHAT_TRANSPORT_H


/**
 * The default read buffer size.
 */
#define DEFAULT_READ_BUFFER_SIZE = -1;

/**
 * The default write buffer size.
 */
#define DEFAULT_WRITE_BUFFER_SIZE = -1;

/**
 * Default read timeout in seconds.
 */
#define DEFAULT_READ_TIMEOUT = 30;

/**
 * Default write timeout in seconds.
 */
#define DEFAULT_WRITE_TIMEOUT = 30;

typedef enum {
    STARTING,
    STARTED,
    PAUSING,
    PAUSED,
    STOPPING,
    STOPPED
} transport_state;


// get_name
// set_name
// get_state
// is_blocking
// configure_blocking
// configure_standalone
// is_standalone
// obtain_processor
// get_processor
// set_processor
// get_processor_selector
// set_processor_selector
// get_memory_manager
// set_memory_manager
// get_io_strategy
// set_io_strategy
// get_read_buffer_size
// set_read_buffer_size
// get_write_buffer_size
// set_write_buffer_size
// get_worker_thread_pool
// get_kernel_thread_pool
// set_worker_thread_pool
// set_kernel_thread_pool
// set_kernel_thread_pool_config
// set_worker_thread_pool_config
// get_kernel_thread_pool_config
// get_worker_thread_pool_config
// get_attribute_builder
// set_attribute_builder
// start
// shutdown
// shutdown_now
// add_shutdown_listener
// pause
// resume
// fire_io_event
// is_stopped
// is_paused
// get_reader
// get_writer
// get_connection_monitoring_config
// get_threadpool_monitoring_config
// get_monitoring_config
// notify_transport_error
// get_read_timeout
// set_read_timeout
// get_write_timeout
// set_write_timeout

#endif //LIGHTCHAT_TRANSPORT_H

