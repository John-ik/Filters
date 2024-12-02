#pragma once


#include <inttypes.h>
#include <stdlib.h>
#include <math.h>


typedef uint32_t filter_data_t;
typedef uint32_t filter_data_size_t;
typedef float filter_result_t;


// ==================== Average arithmetic ====================
/**
 * @brief Average arithmetic
 * @param arr pointer to array
 * @param size integer size of array
 */
filter_result_t filter_avr(filter_data_t* arr, filter_data_size_t size);


// ==================== Average arithmetic pull ====================
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


// ==================== Running average arithmetic ====================
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


// ==================== Exponential running ====================
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


// ==================== Exponential running adaptive ====================
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


// ==================== Median on 3 values ====================
/**
 * @brief init as `{0}`
 */
typedef struct {
    filter_data_t buf[3];
    filter_data_size_t indx;
} filter_median3_t;

/**
 * @brief Median filter. Select average from stored 2 values and new one
 * @param data init as `{0}`
 */
filter_data_t filter_median3 (filter_median3_t* data, filter_data_t new_data);


// ==================== Median ====================
/**
 * @param size the number of stored values
 */
typedef struct {
    filter_data_size_t size;
    filter_data_t* buf;
    filter_data_size_t indx;
    uint8_t init;
} filter_median_t;

/**
 * @brief Median filter. Select average from stored `size-1` values and new one
 * @param data init as `{size}`
 */
filter_data_t filter_median (filter_median_t* data, filter_data_t new_data);


// ==================== Simple one-dimension Kalman ====================
/**
 * @param err_measure approximate noise
 * @param q rate of change of values (0.001 : 1)
 * @param err_estimate error estimate (not necessary. will set equal err_measure on init)
 */
typedef struct {
    float err_measure;  // примерный шум измерений
    float q;   // скорость изменения значений 0.001-1, варьировать самому
    float err_estimate;
    float last_estimate;
} filter_simple_kalman_t;

/**
 * @brief Simple one-dimensional Kalman
 */
filter_result_t filter_simple_kalman (filter_simple_kalman_t* data, float new_data);


// ==================== Alpha Beta ====================
/**
 * @brief USE `filter_alpha_beta_init`
 */
typedef struct {
    float dt;
    float xk_1, vk_1, a, b;
    float xk, vk, rk;
    float xm;
} filter_alpha_beta_t;

/**
 * @brief configuration function for AlphaBeta filter. Run once for one filter
 * @param delta sampling period (measurements)
 * @param sigma_process process variation
 * @param sigma_noise noise variation
 */
filter_alpha_beta_t filter_alpha_beta_init (float delta, float sigma_process, float sigma_noise);

/**
 * @brief Alpha Beta filter
 * https://en.wikipedia.org/wiki/Alpha_beta_filter
 */
float filter_alpha_beta (filter_alpha_beta_t* data, float new_data);
