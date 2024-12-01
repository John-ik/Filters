#pragma once


#include <inttypes.h>
#include <stdlib.h>


typedef uint32_t filter_data_t;
typedef uint32_t filter_data_size_t;
typedef float filter_result_t;


/**
 * @brief Average arithmetic
 * @param arr pointer to array
 * @param size integer size of array
 */
filter_result_t filter_avr(filter_data_t* arr, filter_data_size_t size);


/**
 * @param size the number of stored values
 */
typedef struct {
    filter_data_size_t size;
    filter_data_size_t counter;
    uint64_t sum; //TODO: 64 bits ???
    filter_result_t prev_result;
} filter_avr_pull_t;

/**
 * @brief Average arithmetic
 * @param data configure `size` field
 * @param new_data value to filter
 */
filter_result_t filter_avr_pull (filter_avr_pull_t* data, filter_data_t new_data);


/**
 * @param size the number of stored values
 */
typedef struct {
    filter_data_size_t size;
    filter_data_size_t indx;
    filter_result_t average;
    filter_result_t* vals;
    uint8_t init;
} filter_run_avr_t;

/**
 * @brief Running average
 * @param data configure `size` field
 * @param new_data value to filter
 */
filter_result_t filter_run_avr (filter_run_avr_t* data, filter_data_t new_data);


/**
 * @param k from 0.0 to 1.0: the smaller the smoother
 */
typedef struct {
    float k;
    filter_result_t val;
} filter_exp_run_t;

/**
 * @brief Exponential running average
 * @param data configure `k` field (0.0 : 1.0)
 * @param new_data value to filter
 */
filter_result_t filter_exp_run (filter_exp_run_t* data, filter_data_t new_data);


/**
 * @param sharpness if delta bigger it, used `k2` else `k1`
 * @param k1 little k
 * @param k2 big k
 */
typedef struct {
    float sharpness;
    float k1;
    float k2;
    float k;
    filter_result_t val;
} filter_exp_run_adapt_t;

/**
 * @brief Adaptive exponential running average
 * @param data configure `sharpness`, `k1`, `k2` fields
 */
filter_result_t filter_exp_run_adapt (filter_exp_run_adapt_t* data, filter_data_t new_data);


/**
 * @brief init as `{0}`
 */
typedef struct {
    filter_data_t buf[3];
    filter_data_size_t indx;
} filter_median3_t;

/**
 * @brief Median filter. Select average from given
 * @param data init as `{0}`
 */
filter_data_t filter_median3 (filter_median3_t* data, filter_data_t new_data);


/**
 * 
 */
typedef struct {
    filter_data_size_t size;
    filter_data_t* buf;
    filter_data_size_t indx;
    uint8_t init;
} filter_median_t;

/**
 * 
 */
filter_data_t filter_median (filter_median_t* data, filter_data_t new_data);
