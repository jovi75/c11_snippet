#include "tsc.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

using namespace std;
using namespace std::chrono;

typedef function<void(uint64_t)> fn;

class tsc {
public:

    explicit tsc(fn f) : fn_(f), tsc_(0) {
        tsc_ = _rdtsc();
    }

    ~tsc() {
        tsc_ = _rdtsc() - tsc_;
        fn_(tsc_);
    }

private:
    static inline uint64_t _rdtsc() {
        uint32_t hi, lo;
        asm volatile ("rdtscp" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
    }

    fn fn_;
    uint64_t tsc_;
};

class NsTime {
public:
    explicit NsTime(fn f) : _fn(f) {
        ::clock_gettime(CLOCK_REALTIME, &_start);
    }
    ~NsTime() {
        struct timespec end;
        ::clock_gettime(CLOCK_REALTIME, &end);
        _fn( 1000000000 * (end.tv_sec - _start.tv_sec) + (end.tv_nsec - _start.tv_nsec) );
    }

private:
    struct timespec _start;
    fn _fn;
};

//在include/asm-i386/Msr.h(内核版本：2.6.23)里可以看到一个函数
//不准确：溢出？
static inline uint64_t native_read_tsc() {
    uint64_t val;
    asm volatile("rdtscp": "=A" (val));
    return val;
}
// static unsigned long long native_read_tsc() {__asm__("rdtscp");}

static inline uint64_t rdtsc(void) {
    uint32_t hi, lo;
    asm volatile ("rdtscp" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#define rdtscll_64(val)\
    do {\
        unsigned int __a,__d;\
        asm volatile("rdtscp": "=a" (__a), "=d" (__d));\
        (val) = (((unsigned long long)__d)<<32) | (__a);\
    } while(false);

// /* calculate cpu_khz */
void init_cpu_khz(void)
{
//    /* report CPU clock rate in Hz.
//     * The formula is (10^6 * 2^32) / (2^32 * 1 / (clocks/us)) =
//     * clock/second. Our precision is about 100 ppm.
//     */
//    unsigned long eax=0, edx=1000;
//    unsigned long cpu_khz;
//    asm volatile ("noop" : "=A" (cpu_khz));
//    asm volatile("divl %2"
//            :"=a" (cpu_khz), "=d" (edx)
//                   :"r" (tsc_quotient),
//                   "0" (eax), "1" (edx));
//     printf("Detected %lu.%03lu MHz processor.\n", cpu_khz / 1000, cpu_khz % 1000);
}


void mz1() {
    unsigned long long tsc1 = native_read_tsc();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    unsigned long long tsc2 = native_read_tsc();
    cout << "tsc pass: " << tsc2 - tsc1 << endl;
    cout << "cpu MZ: " << (tsc2 - tsc1)/1000000 << endl;
}


void mz2() {
    unsigned long long tsc1 = rdtsc();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    unsigned long long tsc2 = rdtsc();
    cout << "tsc pass: " << tsc2 - tsc1 << endl;
    cout << "cpu MZ: " << (tsc2 - tsc1)/1000000 << endl;

    init_cpu_khz();

    // ClockTimestamp t = ClockTimestamp::now();
    // cout << "real time " << t.ms() << "ms " << t.ns() << "ns" << endl;

    // NsTime ns([](uint64_t v) {
    //     cout << "ns:" << v << " ms:" << v/1000 << endl;
    // });

    // tsc tc([](uint64_t v) {
    //     cout << "tsc pass: " << v << endl;
    //     cout << "cpu MZ: " << v*2 / 1000000 << endl;
    // });

    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

float readCpuFreq() {
    char cmd_dmesg[] = "dmesg | grep -e 'Detected' | grep -e 'processor' | sed -e 's/.*\\s\\+\\([.0-9]\\+\\)\\s\\+MHz.*/\\1/'";
    FILE *pd = popen(cmd_dmesg, "r");
    assert(pd);
    float freq;
    int nc = fscanf(pd, "%f", &freq);
    assert(nc == 1 && freq > 0);
    pclose(pd);
    return freq;
}


void mz3() {
    unsigned long long tsc1, tsc2;
    rdtscll_64(tsc1);
    //::sleep(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    rdtscll_64(tsc2);
    cout << "tsc pass: " << tsc2 - tsc1 << endl;
    cout << "cpu MZ: " << (tsc2 - tsc1)/1000000 << endl;
}

int main() {
    // mz1();
    mz2();
    mz3();

    // double freq = readCpuFreq();
    // printf("dmesg:%.3f\n", freq);
    // cout << "dmesg: " << freq << endl;
}
