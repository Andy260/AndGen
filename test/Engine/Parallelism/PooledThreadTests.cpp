#include <Engine/Parallelism/PooledThread.hpp>

// STL includes
#include <chrono>
#include <memory>
// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>
// AndGen test includes
#include "TimedJob.hpp"
// Google Test includes
#include <gtest/gtest.h>

namespace AndGen::Tests
{
	/// <summary>
	/// Test fixture, which controls clean-up of created resources within tests
	/// </summary>
	class PooledThreadTests : public ::testing::Test
	{
	protected:
		static std::vector<std::shared_ptr<TimedJob>> m_jobs;
		static std::unique_ptr<PooledThread> m_pooledThread;

		/// <summary>
		/// Per-test Tear Down
		/// </summary>
		virtual void TearDown() override final
		{
			// Ensure all created tests will finish executing
			for (size_t i = 0; i < m_jobs.size(); i++)
			{
				m_jobs[i]->canExecute = true;
			}
			m_jobs.clear();

			// Destroy pooled thread
			delete m_pooledThread.release();
		}

		/// <summary>
		/// Creates a job which will be cleaned up after test case execution
		/// </summary>
		/// <returns>
		/// Created Job
		/// </returns>
		static std::shared_ptr<TimedJob> CreateJob()
		{
			std::shared_ptr<TimedJob> job = std::make_shared<TimedJob>();
			m_jobs.push_back(job);

			return job;
		}
	};
	std::vector<std::shared_ptr<TimedJob>> PooledThreadTests::m_jobs;
	std::unique_ptr<PooledThread> PooledThreadTests::m_pooledThread;

	// Normal usage of QueueJob()
	TEST_F(PooledThreadTests, QueueJob)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();

		// Create Jobs to execute
		std::shared_ptr<TimedJob> firstJob	= CreateJob();
		std::shared_ptr<TimedJob> secondJob = CreateJob();
		// Ensure Jobs execute
		firstJob->canExecute	= true;
		secondJob->canExecute	= true;

		// Queue test jobs
		m_pooledThread->QueueJob<TimedJob>(firstJob);
		m_pooledThread->QueueJob<TimedJob>(secondJob);
		m_pooledThread->Start();

		// Wait for pooled thread to execute
		m_pooledThread->WaitForQueue();
		m_pooledThread->Stop(true);

		// Ensure test jobs are completed
		ASSERT_TRUE(firstJob->IsCompleted());
		ASSERT_TRUE(secondJob->IsCompleted());
		// Ensure jobs were completed in queued order
		ASSERT_TRUE(firstJob->endOfExecution <= secondJob->startOfExecution);
	}

	// QueueJob() with a null job
	TEST_F(PooledThreadTests, QueueJob_Null)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();

		// Attempt to queue null job
		m_pooledThread->QueueJob<TimedJob>(std::shared_ptr<TimedJob>(nullptr));

		// Ensure null job wasn't queued
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 0);
	}

	// Normal usage of Start()
	TEST_F(PooledThreadTests, Start)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();
		// Start pooled thread
		m_pooledThread->Start();

		// Give enough time for thread to start
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread is running
		ASSERT_EQ(m_pooledThread->GetStatus(), AndGen::PooledThread::Status::Idle);
	}

	// Usage of Start() when thread is already running
	TEST_F(PooledThreadTests, Start_AlreadyRunning)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();
		// Start pooled thread
		m_pooledThread->Start();

		// Give enough time for thread to start
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread is running
		ASSERT_EQ(m_pooledThread->GetStatus(), AndGen::PooledThread::Status::Idle);

		// Attempt to start pooled thread when already running
		m_pooledThread->Start();
		// Ensure pooled thread is still running
		ASSERT_EQ(m_pooledThread->GetStatus(), AndGen::PooledThread::Status::Idle);
	}

	// Normal usage of Stop()
	TEST_F(PooledThreadTests, Stop)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();
		// Start pooled thread
		m_pooledThread->Start();

		// Give enough time for thread to start
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread is running
		ASSERT_EQ(m_pooledThread->GetStatus(), AndGen::PooledThread::Status::Idle);

		// Stop pooled thread
		m_pooledThread->Stop(true);
		// Give enough time for thread to stop
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// Ensure pooled thread has stopped
		ASSERT_EQ(m_pooledThread->GetStatus(), AndGen::PooledThread::Status::Stopped);
	}

	// Usage of Stop() when thread isn't running
	TEST_F(PooledThreadTests, Stop_NotRunning)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();

		// Attempt to stop thread when not running
		m_pooledThread->Stop(true);

		// Ensure thread isn't running
		ASSERT_EQ(m_pooledThread->GetStatus(), AndGen::PooledThread::Status::Stopped);
	}

	// Normal usage of WaitForQueue()
	TEST_F(PooledThreadTests, WaitForQueue)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();

		// Create Jobs to execute
		std::shared_ptr<TimedJob> job = CreateJob();
		// Ensure Jobs execute
		job->canExecute = true;

		// Queue test jobs
		m_pooledThread->QueueJob<TimedJob>(job);
		m_pooledThread->Start();
		// Ensure jobs were added to queue
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 1);

		// Wait for pooled thread to execute
		m_pooledThread->WaitForQueue();

		// Ensure thread has completed all jobs
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 0);
	}

	// Usage of WaitForQueue() with an empty queue
	TEST_F(PooledThreadTests, WaitForQueue_Empty)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();
		// Start thread
		m_pooledThread->Start();

		// Wait for queue with no jobs
		m_pooledThread->WaitForQueue();
	}

	// Usage of WaitForQueue() with thread not started
	TEST_F(PooledThreadTests, WaitForQueue_NotStarted)
	{
		// Create Pooled Thread
		m_pooledThread = std::make_unique<PooledThread>();

		// Create Jobs to execute
		std::shared_ptr<TimedJob> job = CreateJob();
		// Ensure Jobs execute
		job->canExecute = true;

		// Queue test jobs
		m_pooledThread->QueueJob<TimedJob>(job);
		// Ensure jobs were added to queue
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 1);

		// Wait for pooled thread to execute
		m_pooledThread->WaitForQueue();

		// Ensure thread hasn't completed all jobs
		// since the thread isn't started yet
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 1);
	}

	// Normal usage of ClearQueue()
	TEST_F(PooledThreadTests, ClearQueue)
	{
		// Create pooled thread
		m_pooledThread = std::make_unique<PooledThread>();
		// Ensure queue is empty
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 0);

		// Create jobs
		std::shared_ptr<TimedJob> firstJob	= std::make_shared<TimedJob>();
		std::shared_ptr<TimedJob> secondJob = std::make_shared<TimedJob>();
		// Add jobs to queue
		m_pooledThread->QueueJob<TimedJob>(firstJob);
		m_pooledThread->QueueJob<TimedJob>(secondJob);
		// Ensure jobs are added to queue
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 2);

		// Clear queue
		m_pooledThread->ClearQueue();

		// Ensure queue is empty
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 0);
	}

	// Usage of ClearQueue with an empty queue
	TEST_F(PooledThreadTests, ClearQueue_Empty)
	{
		// Create pooled thread
		m_pooledThread = std::make_unique<PooledThread>();
		// Ensure queue is empty
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 0);

		// Clear queue
		m_pooledThread->ClearQueue();
		// Ensure queue is still empty
		ASSERT_EQ(m_pooledThread->GetQueue().Count(), 0);
	}

	// GetStatus() with thread idle
	TEST_F(PooledThreadTests, GetStatus_Idle)
	{
		// Create thread and start it
		m_pooledThread = std::make_unique<PooledThread>();
		m_pooledThread->Start();

		// Ensure status of thread is as expected
		ASSERT_EQ(m_pooledThread->GetStatus(), PooledThread::Status::Idle);
	}

	// GetStatus() with thread executing jobs
	TEST_F(PooledThreadTests, GetStatus_ExecutingJobs)
	{
		// Create thread and start it
		m_pooledThread = std::make_unique<PooledThread>();
		m_pooledThread->Start();

		// Create job and begin executing it within the thread
		std::shared_ptr<TimedJob> job	= CreateJob();
		job->canExecute					= false;
		m_pooledThread->QueueJob(job);

		// Wait for thread to begin executing the job
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Ensure status of thread is as expected
		ASSERT_EQ(m_pooledThread->GetStatus(), PooledThread::Status::ExecutingJobs);
	}

	// GetStatus() with thread stopped
	TEST_F(PooledThreadTests, GetStatus_Stopped)
	{
		// Create thread
		m_pooledThread = std::make_unique<PooledThread>();

		// Ensure status of thread is as expected
		ASSERT_EQ(m_pooledThread->GetStatus(), PooledThread::Status::Stopped);
	}
}
