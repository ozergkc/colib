#pragma once

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

/* 
Options:
Please define the options you like, the options are mutually exclusive
    CO_CIRCULAR_BUFFER_THREAD_SAFE
        If defined enables thread safe implementation
        you must provide a mutex lock and unlock function
    CO_CIRCULAR_BUFFER_OVERWRITE_CALLBACK
        If defined provides override callback which user must provide
        The callback is called after memory is overwritten - can be used for error detection. 
    CO_CIRCULAR_BUFFER_DISALLOW_OVERWRITE
        If enabled prefers data in buffer over new data.
    CO_CIRCULAR_BUFFER_MANAGE_MEMORY
        If defined allocates memory from heap itself and releases upon terminate

*/

#ifdef __cplusplus
#extern "C"
{
#endif

// status inquiry
size_t co_circular_buffer_get_empty();
size_t co_circular_buffer_get_size();
bool co_circular_buffer_is_empty();
bool co_circular_buffer_is_full();

#ifdef CO_CIRCULAR_BUFFER_MANAGE_MEMORY
// allocate the memory and initialize the buffer
void co_circular_buffer_init(size_t buffer_capacity);
void co_circular_buffer_terminate();
#else
// initialize with your buffer
void co_circular_buffer_init(uint8_t* buffer, size_t buffer_capacity);
void co_circular_buffer_terminate();
#endif

// insert data
void co_circular_buffer_push(uint8_t byte);
void co_circular_buffer_push_arr(uint8_t* byte, size_t size);

// remove data, returns popped size
size_t co_circular_buffer_pop(uint8_t* byte);
size_t co_circular_buffer_pop_arr(uint8_t* buf, size_t size);

#ifdef __cplusplus
)
#endif