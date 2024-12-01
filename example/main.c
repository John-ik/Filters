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
        filter_avr_pull_t f1 = {3, 0,};
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


    // 
}
