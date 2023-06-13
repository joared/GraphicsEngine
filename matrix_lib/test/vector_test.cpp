#include <gtest/gtest.h>
#include <chrono>
#include "vector.h"

namespace Utils
{
    class Timer
    {
        public:
            Timer() = default;
            void start();
            int64_t stop();
        private:
            std::chrono::_V2::system_clock::time_point m_start;
            int64_t m_elapsed;
    };
    void Timer::start()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    int64_t Timer::stop()
    {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - m_start);
        m_elapsed = duration.count();
        
        return m_elapsed;
    }
}

TEST(VectorTest, PerfomanceTest)
{
    Utils::Timer timer;
    int n = 1000000;

    timer.start();

    matx::Vector3d temp;
    for (int i=0; i<n; i++)
    {
        auto vec = matx::Matrix33d({1,2,3,4,5,6,7,8,9}) * matx::Vector3d({1,2,3});
        temp = vec;
    }

    auto elapsed = timer.stop();
    auto elapsedPerMult = elapsed / (double)n;
    std::cout << "Elapsed (n = " << n << "): " + std::to_string(elapsed / 1000000.0) + "s" << std::endl;
    std::cout << "Elapsed per multiplication: " + std::to_string(elapsedPerMult) + " micros." << std::endl;
};

TEST(VectorTest, DotProduct)
{
    matx::Vector3d v1{1,2,3};
    matx::Vector3d v2{4,5,6};
    ASSERT_EQ(32, v1*v2);
}