#include <Engine/Parallelism/ThreadPool.hpp>

// STL includes
#include <algorithm>
#include <thread>
// Google Test includes
#include <gtest/gtest.h>

namespace AndGen::Tests
{
	// Normal usage of GetIdealThreadCount()
	TEST(ThreadPoolTests, GetIdealThreadCount)
	{
		// Ensure ideal thread count is:
		// Hardware thread count - 1 (because of main thread being a separate thread)
		ASSERT_EQ(ThreadPool::GetIdealThreadCount(),
			std::max<unsigned int>(0, std::thread::hardware_concurrency() - 1));
	}
}
