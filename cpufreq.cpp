// cpufreq库可在/usr/lib目录下找到
// 编译: g++ -g -o x x.cpp -lcpufreq
// 需要以root用户执行以下代码
//#include <cpufreq.h>
#include <stdio.h>
#include <tuple>
#include <type_traits>
#include <sys/sysinfo.h> // get_nprocs

// 如果不存在/usr/include/cpufreq.h
#ifndef _CPUFREQ_H
    extern "C" int cpufreq_cpu_exists(unsigned int cpu);
    extern "C" unsigned long cpufreq_get_freq_kernel(unsigned int cpu);
    extern "C" unsigned long cpufreq_get_freq_hardware(unsigned int cpu);
    extern "C" int cpufreq_get_hardware_limits(unsigned int cpu, unsigned long *min, unsigned long *max);
#endif

enum class EnTest {
    enOne, enTwo, enThree
};
using TestTuple = std::tuple<std::string, std::size_t>;

template<typename E>
constexpr typename std::underlying_type<E>::type
toUType(E en) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(en);
}

int main()
{
    // testDeclType();

    TestTuple test;

    toUType(EnTest::enTwo);

    auto val = std::get<toUType(EnTest::enOne)>(test);

    // 取得cpu core的个数，proc是processor的意思
    int nprocs = get_nprocs();
    for (int i=0; i<nprocs; ++i)
    {
        if (0 == cpufreq_cpu_exists(i))
        {
            unsigned long min_freq = 0;
            unsigned long max_freq = 0;
            cpufreq_get_hardware_limits(i, &min_freq, &max_freq);

            printf("cpu[%d]:\n", i);
            printf("min_freq: %lu, max_freq: %lu\n", min_freq, max_freq);
            printf("kernel freq: %lu, hardware freq: %lu\n", cpufreq_get_freq_kernel(i), cpufreq_get_freq_hardware(i));
            printf("\n");
        }
    }

    return 0;
}