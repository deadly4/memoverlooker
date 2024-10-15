#include <stdio.h>

#include "memcheck.h"

int main(void) {
    const char* ps1 = "ath10k_tx_work";
    const char* ps2 = "ath10k_htt_rx_h_mpdu";
    const char* ps3 = "ath10k_tdma_txd_pool_init";
    const char* ps4 = "ath10k_rx_mpdu_frame";
    const char* ps5 = "LPA_Send";
    const char* ps6 = "mtpDestroy";
    const char* ps7 = "mtpCreate";

    memcheck();

    void* ptr1 = memcheck_malloc((size_t)133, ps1);
    void* ptr2 = memcheck_malloc((size_t)777, ps2);
    void* ptr3 = memcheck_malloc((size_t)1488, ps3);
    void* ptr4 = memcheck_malloc((size_t)666, ps4);
    void* ptr5 = memcheck_malloc((size_t)555, ps5);
    void* ptr7 = memcheck_malloc((size_t)100, ps7);
    
    void* ptr_array[10];
    for (int i = 0; i < 10; i++)
    {
        ptr_array[i] = memcheck_malloc(i + 1, ps7);
    }

    memcheck_free(ptr1, ps1);
    memcheck_free(ptr2, ps2);
    memcheck_free(ptr3, ps3);
    memcheck_free(ptr4, ps4);
    memcheck_free(ptr5, ps6);
    memcheck_free(ptr7, ps7);

    for (int i = 0; i < 10; i++)
    {
        memcheck_free(ptr_array[i], ps3);
    }

    memcheck_report();
    memcheck_destroy();


    return 0;
}