#include "filters.h"



filter_result_t filter_avr(filter_data_t* arr, filter_data_size_t size){
    uint64_t sum = 0; //TODO: такой размер? или обобщить?
    for (filter_data_size_t i = 0; i < size; i++)
        sum += arr[i];
    return sum / size;
}


// Среднее арифметическое растянутая выборка
filter_result_t filter_avr_pull (filter_avr_pull_t* data, filter_data_t new_data){
    data->sum += new_data;
    data->counter++;
    if (data->counter == data->size)    {
        data->prev_result = data->sum / data->size;
        data->sum = 0;
        data->counter = 0;
    }
    return data->prev_result;
}


filter_result_t filter_run_avr (filter_run_avr_t* data, filter_data_t new_data){
    if (!data->init){
        data->vals = calloc(data->size, sizeof (*(data->vals)));
        data->init = 1;
    }
    if (++data->indx >= data->size) data->indx = 0;
    data->average -= data->vals[data->indx];
    data->average += new_data;
    data->vals[data->indx] = new_data;
    return ((filter_result_t) data->average / data->size);
}


filter_result_t filter_exp_run (filter_exp_run_t* data, filter_data_t new_data){
    data->val += (new_data - data->val) * data->k;
    return data->val;
}


filter_result_t filter_exp_run_adapt (filter_exp_run_adapt_t* data, filter_data_t new_data){
    if ((new_data > data->val ? new_data - data->val : data->val - new_data) > data->sharpness)
        data->k = data->k2; 
    else
        data->k = data->k1;
    data->val += (new_data - data->val) * data->k;
    return data->val;
}


filter_data_t filter_median3 (filter_median3_t* data, filter_data_t new_data){
    data->buf[data->indx] = new_data;
    if (++data->indx >= 3) data->indx = 0;
    filter_data_t a = data->buf[0], b = data->buf[1], c = data->buf[2];
    return (a < b) ? ((b < c) ? b : ((c < a) ? a : c)) : ((a < c) ? a : ((c < b) ? b : c));
}


filter_data_t filter_median (filter_median_t* data, filter_data_t new_data){
    if (!data->init){
        data->buf = calloc(data->size, sizeof(*data->buf));
        data->init = 1;
    }
    data->buf[data->indx] = new_data;
    if ((data->indx < data->size - 1) && (data->buf[data->indx] > data->buf[data->indx + 1])){
        for (filter_data_size_t i = data->indx; i < data->size - 1; i++){
            if (data->buf[i] > data->buf[i + 1]){
                filter_data_t buff = data->buf[i];
                data->buf[i] = data->buf[i + 1];
                data->buf[i + 1] = buff;
            }
        }
    }
    else{
        if ((data->indx > 0) && (data->buf[data->indx - 1] > data->buf[data->indx])){
            for (filter_data_size_t i = data->indx; i > 0; i--){
                if (data->buf[i] < data->buf[i - 1]){
                    filter_data_t buff = data->buf[i];
                    data->buf[i] = data->buf[i - 1];
                    data->buf[i - 1] = buff;
                }
            }
        }
    }
    if (++data->indx >= data->size)
        data->indx = 0;
    return data->buf[(filter_data_size_t)data->size / 2];
}


float filter_simple_kalman (filter_simple_kalman_t* data, float new_data){
    float kalman_gain, current_estimate;
    // first init err_estimate
    if (data->err_estimate == 0) data->err_estimate = data->err_measure;

    kalman_gain = (float)data->err_estimate / (data->err_estimate + data->err_measure);
    current_estimate = data->last_estimate + (float)kalman_gain * (new_data - data->last_estimate);
    data->err_estimate =  (1.0 - kalman_gain) * data->err_estimate + fabs(data->last_estimate - current_estimate) * data->q;
    data->last_estimate = current_estimate;
    return current_estimate;
}


filter_alpha_beta_t filter_alpha_beta_init (float delta, float sigma_process, float sigma_noise){
    filter_alpha_beta_t data = {0};
    data.dt = delta;
    float lambda = (float)sigma_process * data.dt * data.dt / sigma_noise;
    float r = (4.0 + lambda - sqrt(8.0 * lambda + lambda * lambda)) / 4.0;
    data.a = 1.0 - r * r;
    data.b = 2.0 * (2.0 - data.a) - 4.0 * sqrt(1.0 - data.a);
    return data;
}


float filter_alpha_beta (filter_alpha_beta_t* data, float new_data){
    data->xm = new_data;
    data->xk = data->xk_1 + (float)data->vk_1 * data->dt;
    data->vk = data->vk_1;
    data->rk = data->xm - data->xk;
    data->xk += (float)data->a * data->rk;
    data->vk += (float)data->b * data->rk / data->dt;
    data->xk_1 = data->xk;
    data->vk_1 = data->vk;
    return data->xk_1;
}
