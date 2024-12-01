#pragma once

#include <inttypes.h>
#include <stdlib.h>


typedef uint32_t filter_data_t;
typedef uint32_t filter_data_size_t;
typedef float filter_result_t;

/**
 * @brief 
 */
filter_result_t filter_avr(filter_data_t* arr, filter_data_size_t size);



typedef struct {
    filter_data_size_t size;
    filter_data_size_t counter;
    filter_result_t prev_result;
    uint64_t sum;
} filter_avr_pull_t;


filter_result_t filter_avr_pull (filter_avr_pull_t* data, filter_data_t newData);

