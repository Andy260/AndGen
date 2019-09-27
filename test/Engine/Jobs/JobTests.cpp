#include <AndGen/Engine/Jobs/Job.hpp>

// Google Test includes
#include <gtest/gtest.h>

namespace AndGen::Tests
{
	class TestJob : public Job
	{
	public:
		TestJob() : Job() {}
		TestJob(const TestJob&) = default;
		~TestJob() = default;

		bool executeWasRan = false;

	protected:
		virtual void Execute() override
		{
			executeWasRan = true;
		}
	};

	// Normal usage of Run()
	TEST(JobTests, Run)
	{
		TestJob testJob;
		testJob.Run();

		ASSERT_TRUE(testJob.IsCompleted());
	}
}
