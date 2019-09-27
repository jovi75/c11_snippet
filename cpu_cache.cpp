#include <inttypes.h>
#include <string.h>
#include <stdio.h>

static inline uint64_t _rdtsc() {
        uint32_t hi, lo;
        asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
}

int main() {
    const int ARR_SIZE = 1024*1024;
    int64_t sum = 0;
    int64_t** data;

    data = new int64_t*[ARR_SIZE];
    for (int i=0; i<ARR_SIZE; ++i) {
        data[i] = new int64_t[8];
        for (int j=0; j<8; ++j) {
            *(data[i] + j) = 1;
        }
    }

    uint64_t tsc1, tsc2;

    tsc1 = _rdtsc();
    for (int i=0; i<ARR_SIZE; ++i) {
        for (int j=0; j<8; ++j) {
            sum += *(data[i]+j);
        }
    }
    tsc2 = _rdtsc();
    printf("sum=%jd, cost %jd\n", sum, tsc2-tsc1);

    tsc1 = _rdtsc();
    for (int i=0; i<8; ++i) {
        for (int j=0; j<ARR_SIZE; ++j) {
            sum += *(data[j]+i);
        }
    }
    tsc2 = _rdtsc();
    printf("sum=%jd, cost %jd\n", sum, tsc2-tsc1);

    return 0;
}