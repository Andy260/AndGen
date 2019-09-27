#include <Engine/Parallelism/PooledThread.hpp>

// STL includes
#include <chrono>
#include <memory>
// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>
// Google Test includes
#include <gtest/gtest.h>

namespace AndGen::Tests
{
	class TestJob : public Job
	{
	public:
		TestJob() : canExecute(false) {}
		TestJob(const TestJob&) = default;
		~TestJob() = default;

		std::atomic_bool canExecute;

		std::chrono::high_resolution_clock::time_point startOfExecution;
		std::chrono::high_resolution_clock::time_point endOfExecution;

	private:
		virtual void Execute() override
		{
			// Record execution start time
			startOfExecution = std::chrono::high_resolution_clock::now();

			// Wait until we can execute
			while (!canExecute) {}

			// Record execution finish time
			endOfExecution = std::chrono::high_resolution_clock::now();
		}
	};

	// Normal usage of QueueJob()
	TEST(PooledThreadTests, QueueJob)
	{
		// Create Jobs to execute
		std::shared_ptr<TestJob> firstJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> secondJob = std::make_shared<TestJob>();
		// Ensure Jobs execute
		firstJob->canExecute = true;
		secondJob->canExecute = true;
		// Create Pooled Thread
		AndGen::PooledThread pooledThread;

		// Queue test jobs
		pooledThread.QueueJob<TestJob>(firstJob);
		pooledThread.QueueJob<TestJob>(secondJob);
		pooledThread.Start();

		// Wait for pooled thread to execute
		pooledThread.WaitForQueue();
		pooledThread.Stop(true);

		// Ensure test jobs are completed
		ASSERT_TRUE(firstJob->IsCompleted());
		ASSERT_TRUE(secondJob->IsCompleted());
		// Ensure jobs were completed in queued order
		ASSERT_TRUE(firstJob->endOfExecution <= secondJob->startOfExecution);
	}

	// QueueJob() with a null job
	TEST(PooledThreadTests, QueueJob_Null)
	{
		// Create Pooled Thread
		AndGen::PooledThread pooledThread;
		// Attempt to queue null job
		pooledThread.QueueJob<TestJob>(std::shared_ptr<TestJob>(nullptr));

		// Ensure null job wasn't queued
		ASSERT_EQ(pooledThread.GetQueue().Count(), 0);
	}

	// Normal usage of Start()
	TEST(PooledThreadTests, Start)
	{
		// Create pooled thread
		AndGen::PooledThread pooledThread;
		// Start pooled thread
		pooledThread.Start();

		// Give enough time for thread to start
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread is running
		ASSERT_TRUE(pooledThread.IsRunning());
	}

	// Usage of Start() when thread is already running
	TEST(PooledThreadTests, Start_AlreadyRunning)
	{
		// Create pooled thread
		AndGen::PooledThread pooledThread;
		// Start pooled thread
		pooledThread.Start();

		// Give enough time for thread to start
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread is running
		ASSERT_TRUE(pooledThread.IsRunning());

		// Attempt to start pooled thread when already running
		pooledThread.Start();
		// Ensure pooled thread is still running
		ASSERT_TRUE(pooledThread.IsRunning());
	}

	// Normal usage of Stop()
	TEST(PooledThreadTests, Stop)
	{
		// Create pooled thread
		AndGen::PooledThread pooledThread;
		// Start pooled thread
		pooledThread.Start();

		// Give enough time for thread to start
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread is running
		ASSERT_TRUE(pooledThread.IsRunning());

		// Stop pooled thread
		pooledThread.Stop(true);
		// Give enough time for thread to stop
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread has stopped
		ASSERT_FALSE(pooledThread.IsRunning());
	}

	// Usage of Stop() when thread isn't running
	TEST(PooledThreadTests, Stop_NotRunning)
	{
		// Create pooled thread
		AndGen::PooledThread pooledThread;

		// Attempt to stop thread when not running
		pooledThread.Stop(true);

		// Ensure thread isn't running
		ASSERT_FALSE(pooledThread.IsRunning());
	}

	// Normal usage of WaitForQueue()
	TEST(PooledThreadTests, WaitForQueue)
	{
		// Create Jobs to execute
		std::shared_ptr<TestJob> job = std::make_shared<TestJob>();
		// Ensure Jobs execute
		job->canExecute = true;
		// Create Pooled Thread
		AndGen::PooledThread pooledThread;

		// Queue test jobs
		pooledThread.QueueJob<TestJob>(job);
		pooledThread.Start();
		// Ensure jobs were added to queue
		ASSERT_EQ(pooledThread.GetQueue().Count(), 1);

		// Wait for pooled thread to execute
		pooledThread.WaitForQueue();

		// Ensure thread has completed all jobs
		ASSERT_EQ(pooledThread.GetQueue().Count(), 0);
	}

	// Usage of WaitForQueue() with an empty queue
	TEST(PooledThreadTests, WaitForQueue_Empty)
	{
		// Create pooled thread
		AndGen::PooledThread pooledThread;
		// Start thread
		pooledThread.Start();

		// Wait for queue with no jobs
		pooledThread.WaitForQueue();
	}

	// Normal usage of ClearQueue()
	TEST(PooledThreadTests, ClearQueue)
	{
		// Create pooled thread
		AndGen::PooledThread pooledThread;
		// Ensure queue is empty
		ASSERT_EQ(pooledThread.GetQueue().Count(), 0);

		// Create jobs
		std::shared_ptr<TestJob> firstJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> secondJob = std::make_shared<TestJob>();
		// Add jobs to queue
		pooledThread.QueueJob<TestJob>(firstJob);
		pooledThread.QueueJob<TestJob>(secondJob);
		// Ensure jobs are added to queue
		ASSERT_EQ(pooledThread.GetQueue().Count(), 2);

		// Clear queue
		pooledThread.ClearQueue();

		// Ensure queue is empty
		ASSERT_EQ(pooledThread.GetQueue().Count(), 0);
	}

	// Usage of ClearQueue with an empty queue
	TEST(PooledThreadTests, ClearQueue_Empty)
	{
		// Create pooled thread
		AndGen::PooledThread pooledThread;
		// Ensure queue is empty
		ASSERT_EQ(pooledThread.GetQueue().Count(), 0);

		// Clear queue
		pooledThread.ClearQueue();
		// Ensure queue is still empty
		ASSERT_EQ(pooledThread.GetQueue().Count(), 0);
	}
}
