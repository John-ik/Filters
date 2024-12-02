#include "./../filters.h"
#include <assert.h>


int main () {
    filter_result_t r;
    
    // filter_avr
    {
        const size_t size = 15;
        filter_data_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        r = filter_avr(arr, size);
        assert(r == 8);
    }

    // filter avr pull
    {
        // configure size of stored values
        filter_avr_pull_t f1 = {3};
        r = filter_avr_pull(&f1, 1);
        r = filter_avr_pull(&f1, 1);
        r = filter_avr_pull(&f1, 1);
        assert(r == 1);
        r = filter_avr_pull(&f1, 3);
        assert(r == 1);
        r = filter_avr_pull(&f1, 5);
        assert(r == 1);
        r = filter_avr_pull(&f1, 4);
        assert(r == 4);
    }

    // filter run avr
    {
        // configure size of stored values
        filter_run_avr_t f = {3};
        r = filter_run_avr(&f, 3);
        assert(r == 1);
        r = filter_run_avr(&f, 6);
        assert(r == 3);
        r = filter_run_avr(&f, 6);
        assert(r = 5);
        r = filter_run_avr(&f, 6);
        assert(r == 6);
        r = filter_run_avr(&f, 3);
        assert(r == 5);
    }

    // filter exponential running
    {
        // configure k
        filter_exp_run_t f = {0.5};
        r = filter_exp_run(&f, 2);
        assert(r == 1);
        r = filter_exp_run(&f, 2);
        assert(r == 1.5);
        r = filter_exp_run(&f, 3);
        assert(r == 2.25);
        r = filter_exp_run(&f, 3);
        assert(r == 2.625);
        r = filter_exp_run(&f, 3);
        assert(r == 2.8125);
    }

    // adaptive filter exponential running
    {
        // configure sharpness, little k, big k
        filter_exp_run_adapt_t f = {1.5, 0.03, 0.9};
    }

    // median filter on 3 values
    {
        filter_median3_t f = {0};
        filter_median3(&f, 1);
        filter_median3(&f, 2);
        r = filter_median3(&f, 5);
        assert(r == 2);
    }

    // universal median filter
    // For this filter need more iteration before it works as expected
    {
        filter_median_t f = {16};
        for (size_t i = 0; i < 16; i++)
            r = filter_median(&f, i);
        for (size_t i = 0; i < 16; i++)
            r = filter_median(&f, i);
        for (size_t i = 0; i < 16; i++)
            r = filter_median(&f, i);
        for (size_t i = 0; i < 16; i++)
            r = filter_median(&f, i);
        assert(r == 8);
    }

    // simple kalman
    {
        // configure err_measure, q
        filter_simple_kalman_t f = {0.8, 0.1};
    }

    // AlphaBeta
    {
        filter_alpha_beta_t f = filter_alpha_beta_init(0.02, 3.0, 0.7);
    }
}
