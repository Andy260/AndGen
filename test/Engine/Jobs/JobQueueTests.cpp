#include <Engine/Jobs/JobQueue.hpp>

// STL includes
#include <atomic>
#include <memory>
#include <thread>
// Google Test includes
#include <gtest/gtest.h>
// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>

namespace AndGen
{
	namespace UnitTests
	{
		class TestJob : public Job
		{
		public:
			TestJob() : Job() {}
			virtual ~TestJob() override {}

			virtual void Execute() override {}
		};

		// AddJob() normal usage
		TEST(JobQueueTests, AddJob)
		{
			// Create Job Queue and test job
			JobQueue jobQueue;
			std::shared_ptr<Job> job;
			job.reset(new TestJob);
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
			std::shared_ptr<Job> firstJob;
			firstJob.reset(new TestJob);
			std::shared_ptr<Job> secondJob;
			secondJob.reset(new TestJob);
			std::shared_ptr<Job> thirdJob;
			thirdJob.reset(new TestJob);

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
		TEST(JobQueueTests, AddJobNull)
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

		// ExecuteNextJob() normal usage
		TEST(JobQueueTests, ExecuteNextJob)
		{
			// Create Job Queue and test job
			JobQueue jobQueue;
			std::shared_ptr<Job> job;
			job.reset(new TestJob);
			// Add test job to Job Queue
			jobQueue.AddJob(job);

			// Execute job queue on worker thread
			// and wait for thread to finish
			jobQueue.ExecuteNextJob();

			// Ensure job was completed
			ASSERT_TRUE(job->IsCompleted());
		}

		// ExecuteNextJob() normal usage with multiple threads
		TEST(JobQueueTests, ExecuteNextJob_Threaded)
		{
			// Create Job Queue and test jobs
			JobQueue jobQueue;
			std::shared_ptr<Job> firstJob;
			firstJob.reset(new TestJob);
			std::shared_ptr<Job> secondJob;
			secondJob.reset(new TestJob);
			std::shared_ptr<Job> thirdJob;
			thirdJob.reset(new TestJob);
			// Add test jobs to Job Queue
			jobQueue.AddJob(firstJob);
			jobQueue.AddJob(secondJob);
			jobQueue.AddJob(thirdJob);

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
	}
}