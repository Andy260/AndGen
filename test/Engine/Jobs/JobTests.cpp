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
		std::shared_ptr<TestJob> testJob = std::make_shared<TestJob>();

		// Ensure job isn't flagged as completed before execution
		ASSERT_FALSE(testJob->IsCompleted());

		// Execute job
		testJob->Run();

		// Ensure job is flagged as completed after execution
		ASSERT_TRUE(testJob->IsCompleted());
	}
}
