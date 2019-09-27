#include <Engine/Jobs/JobQueue.hpp>

// STL includes
#include <atomic>
#include <functional>
#include <memory>
#include <thread>
// Google Test includes
#include <gtest/gtest.h>
// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>

namespace AndGen::Tests
{
	class TestJob : public Job
	{
	public:
		TestJob() : Job() {}
		virtual ~TestJob() override {}

		virtual void Execute() override final
		{
			// Record execution time
			executionTime = std::chrono::high_resolution_clock::now();
		}

		std::chrono::high_resolution_clock::time_point executionTime;
	};

	class TestHelper
	{
	public:
		static std::shared_ptr<Job> CreateJob()
		{
			std::shared_ptr<Job> job;
			job.reset(new TestJob);

			return job;
		}

		static JobQueue CreateQueue(size_t jobsCount)
		{
			AndGen::JobQueue jobQueue;

			for (size_t i = 0; i < jobsCount; i++)
			{
				std::shared_ptr<Job> job = CreateJob();
				jobQueue.AddJob(job);
			}

			return jobQueue;
		}
	};

	// AddJob() normal usage
	TEST(JobQueueTests, AddJob)
	{
		// Create Job Queue and test job
		JobQueue jobQueue;
		std::shared_ptr<Job> job = TestHelper::CreateJob();
		// Ensure Job Queue has expected values after creation
		ASSERT_EQ(jobQueue.Count(), 0);

		// Add test job to job queue and ensure
		// job queue has expected values
		jobQueue.AddJob(job);
		ASSERT_EQ(jobQueue.Count(), 1);
	}

	// AddJob() normal usage with multiple threads
	TEST(JobQueueTests, AddJob_Threaded)
	{
		// Create Job Queue
		JobQueue jobQueue;
		// Ensure Job Queue has expected values after creation
		ASSERT_EQ(jobQueue.Count(), 0);

		// Create Test jobs
		std::shared_ptr<Job> firstJob = TestHelper::CreateJob();
		std::shared_ptr<Job> secondJob = TestHelper::CreateJob();
		std::shared_ptr<Job> thirdJob = TestHelper::CreateJob();

		// Add test job to job queue with multiple threads
		auto functor = [&jobQueue](std::shared_ptr<Job> job)
		{
			jobQueue.AddJob(job);
		};

		// Start threads
		std::thread firstThread(functor, firstJob);
		std::thread secondThread(functor, secondJob);
		std::thread thirdThread(functor, thirdJob);
		// Wait for threads to complete
		if (firstThread.joinable())
		{
			firstThread.join();
		}
		if (secondThread.joinable())
		{
			secondThread.join();
		}
		if (thirdThread.joinable())
		{
			thirdThread.join();
		}

		// Ensure job queue has expected values
		ASSERT_EQ(jobQueue.Count(), 3);
	}

	// AddJob() with a null pointer
	TEST(JobQueueTests, AddJob_Null)
	{
		// Create Job Queue and test job
		JobQueue jobQueue;
		std::shared_ptr<Job> job = nullptr;
		// Ensure Job Queue has expected values after creation
		ASSERT_EQ(jobQueue.Count(), 0);

		// Add null job and ensure job count hasn't changed
		jobQueue.AddJob(job);
		ASSERT_EQ(jobQueue.Count(), 0);
	}

	// AddJobQueue() normal usage
	TEST(JobQueueTests, AddJobQueue)
	{
		// Create Job Queue and test job
		JobQueue jobQueue;
		// Ensure Job Queue has expected values after creation
		ASSERT_EQ(jobQueue.Count(), 0);

		// Create Job Queue to add
		JobQueue queueToAdd = TestHelper::CreateQueue(2);
		// Add recently created queue to Job Queue
		jobQueue.AddJobQueue(queueToAdd);

		// Ensure job queue has expected count
		ASSERT_EQ(jobQueue.Count(), queueToAdd.Count());
	}

	// AddJobQueue() normal usage with multiple threads
	TEST(JobQueueTests, AddJobQueue_Threaded)
	{
		// Create Job Queue
		JobQueue jobQueue;
		// Ensure Job Queue has expected values after creation
		ASSERT_EQ(jobQueue.Count(), 0);

		// Add job queues with threads
		auto functor = [&jobQueue]
		{
			// Create queue to add
			JobQueue queueToAdd = TestHelper::CreateQueue(2);
			// Add queue
			jobQueue.AddJobQueue(queueToAdd);
		};
		std::thread firstThread(functor);
		std::thread secondThread(functor);

		// Wait for threads to complete
		if (firstThread.joinable())
		{
			firstThread.join();
		}
		if (secondThread.joinable())
		{
			secondThread.join();
		}

		// Ensure job queue has expected job count
		ASSERT_EQ(jobQueue.Count(), 4);
	}

	// AddJobQueue() with an empty job queue
	TEST(JobQueueTests, AddJobQueue_Empty)
	{
		// Create Job Queue and test job
		JobQueue jobQueue;
		// Ensure Job Queue has expected values after creation
		ASSERT_EQ(jobQueue.Count(), 0);

		// Create empty queue and attempt to add it
		JobQueue emptyQueue;
		jobQueue.AddJobQueue(emptyQueue);

		// Ensure job queue count is as expected
		ASSERT_EQ(jobQueue.Count(), 0);
	}

	// ExecuteNextJob() normal usage with AddJob() providing input
	TEST(JobQueueTests, ExecuteNextJob_AddJob)
	{
		// Create Job Queue and test jobs
		JobQueue jobQueue;
		std::shared_ptr<TestJob> firstJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> secondJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> thirdJob = std::make_shared<TestJob>();
		// Add test jobs to Job Queue
		jobQueue.AddJob(std::dynamic_pointer_cast<Job>(firstJob));
		jobQueue.AddJob(std::dynamic_pointer_cast<Job>(secondJob));
		jobQueue.AddJob(std::dynamic_pointer_cast<Job>(thirdJob));

		// Execute job queue on worker thread
		// and wait for thread to finish
		jobQueue.ExecuteNextJob();
		jobQueue.ExecuteNextJob();
		jobQueue.ExecuteNextJob();

		// Ensure jobs were completed
		ASSERT_TRUE(firstJob->IsCompleted());
		ASSERT_TRUE(secondJob->IsCompleted());
		ASSERT_TRUE(thirdJob->IsCompleted());
		// Ensure jobs were completed in expected order
		ASSERT_TRUE(firstJob->executionTime < secondJob->executionTime &&
			firstJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(secondJob->executionTime > firstJob->executionTime&&
			secondJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(thirdJob->executionTime > firstJob->executionTime&&
			thirdJob->executionTime > secondJob->executionTime);
	}

	// ExecuteNextJob() normal usage with AddJob() providing input
	// and with multiple thread execution
	TEST(JobQueueTests, ExecuteNextJob_AddJob_Threaded)
	{
		// Create Job Queue and test jobs
		JobQueue jobQueue;
		std::shared_ptr<TestJob> firstJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> secondJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> thirdJob = std::make_shared<TestJob>();
		// Add test jobs to Job Queue
		jobQueue.AddJob(std::dynamic_pointer_cast<Job>(firstJob));
		jobQueue.AddJob(std::dynamic_pointer_cast<Job>(secondJob));
		jobQueue.AddJob(std::dynamic_pointer_cast<Job>(thirdJob));

		// Execute job queue on multiple threads
		auto functor = [&jobQueue]()
		{
			jobQueue.ExecuteNextJob();
		};
		// Start threads
		std::thread firstThread(functor);
		std::thread secondThread(functor);
		std::thread thirdThread(functor);
		// Wait for threads to complete
		if (firstThread.joinable())
		{
			firstThread.join();
		}
		if (secondThread.joinable())
		{
			secondThread.join();
		}
		if (thirdThread.joinable())
		{
			thirdThread.join();
		}

		// Ensure all jobs were completed
		ASSERT_TRUE(firstJob->IsCompleted());
		ASSERT_TRUE(secondJob->IsCompleted());
		ASSERT_TRUE(thirdJob->IsCompleted());
		// Ensure jobs were completed in expected order
		ASSERT_TRUE(firstJob->executionTime < secondJob->executionTime &&
			firstJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(secondJob->executionTime > firstJob->executionTime&&
			secondJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(thirdJob->executionTime > firstJob->executionTime&&
			thirdJob->executionTime > secondJob->executionTime);
	}

	// ExecuteNextJob() normal usage with AddQueue() providing input
	TEST(JobQueueTests, ExecuteNextJob_AddQueue)
	{
		// Create Job Queue and test jobs
		JobQueue jobQueue, queueToAdd;
		std::shared_ptr<TestJob> firstJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> secondJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> thirdJob = std::make_shared<TestJob>();
		// Add test jobs to Job Queue to be added
		queueToAdd.AddJob(std::dynamic_pointer_cast<Job>(firstJob));
		queueToAdd.AddJob(std::dynamic_pointer_cast<Job>(secondJob));
		queueToAdd.AddJob(std::dynamic_pointer_cast<Job>(thirdJob));
		// Add job queue to queue which to test
		jobQueue.AddJobQueue(queueToAdd);

		// Execute job queue on worker thread
		// and wait for thread to finish
		jobQueue.ExecuteNextJob();
		jobQueue.ExecuteNextJob();
		jobQueue.ExecuteNextJob();

		// Ensure jobs were completed
		ASSERT_TRUE(firstJob->IsCompleted());
		ASSERT_TRUE(secondJob->IsCompleted());
		ASSERT_TRUE(thirdJob->IsCompleted());
		// Ensure jobs were completed in expected order
		ASSERT_TRUE(firstJob->executionTime < secondJob->executionTime &&
			firstJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(secondJob->executionTime > firstJob->executionTime&&
			secondJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(thirdJob->executionTime > firstJob->executionTime&&
			thirdJob->executionTime > secondJob->executionTime);
	}

	// ExecuteNextJob() normal usage with AddQueue() providing input
	// and with multiple thread execution
	TEST(JobQueueTests, ExecuteNextJob_AddQueue_Threaded)
	{
		// Create Job Queue and test jobs
		JobQueue jobQueue, queueToAdd;
		std::shared_ptr<TestJob> firstJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> secondJob = std::make_shared<TestJob>();
		std::shared_ptr<TestJob> thirdJob = std::make_shared<TestJob>();
		// Add test jobs to Job Queue to be added
		queueToAdd.AddJob(std::dynamic_pointer_cast<Job>(firstJob));
		queueToAdd.AddJob(std::dynamic_pointer_cast<Job>(secondJob));
		queueToAdd.AddJob(std::dynamic_pointer_cast<Job>(thirdJob));
		// Add queue to be added to test job queue
		jobQueue.AddJobQueue(queueToAdd);

		// Execute job queue on multiple threads
		auto functor = [&jobQueue]()
		{
			jobQueue.ExecuteNextJob();
		};
		// Start threads
		std::thread firstThread(functor);
		std::thread secondThread(functor);
		std::thread thirdThread(functor);
		// Wait for threads to complete
		if (firstThread.joinable())
		{
			firstThread.join();
		}
		if (secondThread.joinable())
		{
			secondThread.join();
		}
		if (thirdThread.joinable())
		{
			thirdThread.join();
		}

		// Ensure all jobs were completed
		ASSERT_TRUE(firstJob->IsCompleted());
		ASSERT_TRUE(secondJob->IsCompleted());
		ASSERT_TRUE(thirdJob->IsCompleted());
		// Ensure jobs were completed in expected order
		ASSERT_TRUE(firstJob->executionTime < secondJob->executionTime &&
			firstJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(secondJob->executionTime > firstJob->executionTime&&
			secondJob->executionTime < thirdJob->executionTime);
		ASSERT_TRUE(thirdJob->executionTime > firstJob->executionTime&&
			thirdJob->executionTime > secondJob->executionTime);
	}

	// ExecuteNextJob() with no jobs
	TEST(JobQueueTests, ExecuteNextJobEmpty)
	{
		// Create Job Queue
		JobQueue jobQueue;

		// Ensure ExecuteNextJob() executes
		jobQueue.ExecuteNextJob();
		// Ensure job count doesn't change
		ASSERT_EQ(jobQueue.Count(), 0);
	}

	// Clear() normal usage
	TEST(JobQueueTests, Clear)
	{
		// Create Jobs
		std::shared_ptr<Job> firstJob = TestHelper::CreateJob();
		std::shared_ptr<Job> secondJob = TestHelper::CreateJob();

		// Create Queue
		AndGen::JobQueue jobQueue;

		// Add jobs to queue
		jobQueue.AddJob(firstJob);
		jobQueue.AddJob(secondJob);
		// Ensure queue has expected count
		ASSERT_EQ(jobQueue.Count(), 2);

		// Clear job queue
		jobQueue.Clear();
		// Ensure queue has expected count
		ASSERT_EQ(jobQueue.Count(), 0);
	}

	// Clear() with no jobs
	TEST(JobQueueTests, Clear_Empty)
	{
		// Create Job Queue
		AndGen::JobQueue jobQueue;
		// Ensure queue count is expected value on creation
		ASSERT_EQ(jobQueue.Count(), 0);

		// Ensure queue count is expected value after empting
		jobQueue.Clear();
		ASSERT_EQ(jobQueue.Count(), 0);
	}

	// IsEmpty() normal usage
	TEST(JobQueueTests, IsEmpty)
	{
		// Create Jobs
		std::shared_ptr<Job> firstJob = TestHelper::CreateJob();
		std::shared_ptr<Job> secondJob = TestHelper::CreateJob();

		// Create Queue
		AndGen::JobQueue jobQueue;

		// Add jobs to queue
		jobQueue.AddJob(firstJob);
		jobQueue.AddJob(secondJob);

		// Ensure IsEmpty() is false
		ASSERT_FALSE(jobQueue.IsEmpty());
	}

	// IsEmpty() with no jobs queued
	TEST(JobQueueTests, IsEmpty_Empty)
	{
		// Create Job Queue
		AndGen::JobQueue jobQueue;

		// Ensure IsEmpty() is true
		ASSERT_TRUE(jobQueue.IsEmpty());
	}
}