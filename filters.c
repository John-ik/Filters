#include "filters.h"


// Среднее арифметичекое
filter_result_t filter_avr(filter_data_t* arr, filter_data_size_t size){
    uint64_t sum = 0; //TODO: такой размер? или обобщить?
    for (filter_data_size_t i = 0; i < size; i++)
        sum += arr[i];
    return sum / size;
}


// Среднее арифметическое растянутая выборка
filter_result_t filter_avr_pull (filter_avr_pull_t* data, filter_data_t newData){
    data->sum += newData;
    data->counter++;
    if (data->counter == data->size)    {
        data->prev_result = data->sum / data->size;
        data->sum = 0;
        data->counter = 0;
    }
    return data->prev_result;
}


// кол-во сохраняемых измерений
const int runAvrSize = 10;

//Бегущее среднее арифметическое
// filter_result_t filter_runAvr (filter_data_t newData){
//     static filter_data_t t = 0;
//     static filter_result_t vals[runAvrSize] = {0,};
//     static filter_result_t average = 0;
//     if (++t >= runAvrSize) t = 0;
//     average -= vals[t];
//     average += newData;
//     vals[t] = newData;
//     return ((filter_result_t) average / runAvrSize);
// }

// коэф фильтрации экспоненциального бегущего среднего,
// от 0.0 до 1.0, чем он меньше, тем плавнее фильтр
const float kExpRun = 0.5;

// Экспоненциальное бегущее среднее
filter_result_t filter_expRunAvg (filter_data_t newData){
    static filter_result_t filVal = 0;
    filVal += (newData - filVal) * kExpRun;
    return filVal;
}

// резкость фильтра зависит от модуля разности значений
const float sharpnessExp = 1.5;

// Экспоненциальное бегущее среднее адаптивное
filter_result_t filter_expRunAvgAdaptive (filter_data_t newData){
    static filter_result_t filVal = 0;
    float k = 0;
    //TODO: abs opt
    //TODO: вынести коэф 
    if ((newData > filVal ? newData - filVal : filVal - newData) > sharpnessExp)
        k = 0.9; 
    else
        k = 0.03;
    filVal += (newData - filVal) * k;
    return filVal;
}


struct Filter_median {
    uint16_t* data;
    uint32_t size;
    uint8_t init;
};
// Медианный фильтр с буфером
filter_data_t filter_median (struct Filter_median data, uint16_t newData){
    if (data.size == 3){
        uint16_t a = *(data.data), b = *(data.data + 1), c = *(data.data + 2);
        uint16_t middle = (a < b) ? ((b < c) ? b : ((c < a) ? a : c)) : ((a < c) ? a : ((c < b) ? b : c));
    }
}
