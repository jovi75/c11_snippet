#include <inttypes.h>
#include <time.h>

class TscTimestamp
{
public:
    TscTimestamp() : _value(0) {}
    explicit TscTimestamp(uint64_t v) : _value(v) {}

    static TscTimestamp now()
    {
        return TscTimestamp(_rdtsc());
    }

private:
    static inline uint64_t _rdtsc() {
        uint32_t hi, lo;
        asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
    }

    uint64_t _value;
};

class ClockTimestamp
{
public:
    explicit ClockTimestamp(uint64_t ns) : _ns(ns)
    { }

    uint64_t ms() const
    {
        return _ns / 1000000;
    }

    uint64_t ns() const
    {
        return _ns;
    }

    static ClockTimestamp now()
    {
        struct timespec t;
        ::clock_gettime(CLOCK_REALTIME, &t);
        return ClockTimestamp((uint64_t)t.tv_sec*1000000000 + t.tv_nsec);
    }

private:
    uint64_t _ns;
};