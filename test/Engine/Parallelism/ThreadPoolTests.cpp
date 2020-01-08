#include <Engine/Parallelism/ThreadPool.hpp>

// STL includes
#include <array>
#include <algorithm>
#include <thread>
// AndGen Tests includes
#include "TimedJob.hpp"
// Google Test includes
#include <gtest/gtest.h>

namespace AndGen::Tests
{
	/// <summary>
	/// Test fixture, which controls clean-up of created resources within tests
	/// </summary>
	class ThreadPoolTests : public ::testing::Test
	{
	protected:
		static std::vector<std::shared_ptr<TimedJob>> m_jobs;
		static std::unique_ptr<ThreadPool> m_threadPool;

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

			// Destroy thread pool, if it was created during
			// the test case
			if (m_threadPool != nullptr)
			{
				delete m_threadPool.release();
			}
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
	std::vector<std::shared_ptr<TimedJob>> ThreadPoolTests::m_jobs;
	std::unique_ptr<ThreadPool> ThreadPoolTests::m_threadPool(nullptr);

	/// <summary>
	/// Parameterised Constructor Test fixture
	/// </summary>
	class Constructor : public ThreadPoolTests, public testing::WithParamInterface<unsigned int> {};

	// Tests GetIdealThreadCount()
	TEST_F(ThreadPoolTests, GetIdealThreadCount)
	{
		// Ensure ideal thread count is:
		// Hardware thread count - 1 (because of main thread being a separate thread)
		ASSERT_EQ(ThreadPool::GetIdealThreadCount(),
			std::max<unsigned int>(0, std::thread::hardware_concurrency() - 1));
	}

	// Full constructor test
	TEST_P(Constructor, FullConstructor)
	{
		// Create Thread Pool
		m_threadPool = std::make_unique<ThreadPool>(GetParam());

		ASSERT_EQ(m_threadPool->Size(), GetParam());
	}
	INSTANTIATE_TEST_CASE_P(ThreadPoolTests, Constructor, ::testing::Range(static_cast<unsigned int>(0), static_cast<unsigned int>(16)));

	// Queuejob() test
	// Doesn't test dependencies with jobs
	TEST_F(ThreadPoolTests, QueueJob)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(4);

		std::array<std::shared_ptr<TimedJob>, 4> jobs;
		m_jobs.reserve(jobs.size());
		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Create job
			jobs[i] = CreateJob();
			// Ensure it will execute
			jobs[i]->canExecute = true;
		}

		// Enqueue jobs to thread pool
		for (size_t i = 0; i < jobs.size(); i++)
		{
			m_threadPool->QueueJob(jobs[i]);
		}

		m_threadPool->WaitForThreads();

		// Ensure execution of jobs is as expected
		std::vector<std::thread::id> executionIds;
		executionIds.reserve(4);

		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Ensure job was completed
			ASSERT_TRUE(jobs[i]->IsCompleted());

			executionIds.push_back(jobs[i]->threadID);
		}
		// Ensure amount of threads used for execution
		// is more than 1, and no more than the total number of threads
		// in the thread pool
		ASSERT_GE(executionIds.size(), 1);
		ASSERT_LE(executionIds.size(), 4);
	}

	// QueueJobs() test
	// Doesn't test dependencies with jobs
	TEST_F(ThreadPoolTests, QueueJobs)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(4);

		std::array<std::shared_ptr<TimedJob>, 4> jobs;
		m_jobs.reserve(jobs.size());
		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Create job
			jobs[i] = CreateJob();
			// Ensure it will execute
			jobs[i]->canExecute = true;
		}

		// Enqueue jobs to thread pool
		m_threadPool->QueueJobs(jobs.begin(), jobs.end());

		m_threadPool->WaitForThreads();

		// Ensure execution of jobs is as expected
		std::vector<std::thread::id> executionIds;
		executionIds.reserve(4);

		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Ensure job was completed
			ASSERT_TRUE(jobs[i]->IsCompleted());

			executionIds.push_back(jobs[i]->threadID);
		}
		// Ensure amount of threads used for execution
		// is equal to the number of threads in the thread pool
		ASSERT_EQ(executionIds.size(), m_threadPool->Size());
	}

	// RunningCount() test
	// with all threads running
	TEST_F(ThreadPoolTests, RunningCount_AllRunning)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(2);

		std::array<std::shared_ptr<TimedJob>, 2> jobs;
		m_jobs.reserve(jobs.size());
		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Create job
			jobs[i] = CreateJob();
		}

		// Enqueue jobs to thread pool
		m_threadPool->QueueJobs(jobs.begin(), jobs.end());

		// Wait for thread to begin executing the jobs
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Ensure running count is as expected while all jobs are running
		ASSERT_EQ(m_threadPool->RunningCount(), jobs.size());
	}

	// RunningCount() test
	// with no threads running
	TEST_F(ThreadPoolTests, RunningCount_NoneRunning)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(2);

		// Ensure running count is zero
		ASSERT_EQ(m_threadPool->RunningCount(), 0);
	}

	// RunningCount() test
	// with no threads running
	TEST_F(ThreadPoolTests, RunningCount_HalfRunning)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(6);

		std::array<std::shared_ptr<TimedJob>, 3> jobs;
		m_jobs.reserve(jobs.size());
		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Create job
			jobs[i] = CreateJob();
		}

		// Enqueue jobs to thread pool
		m_threadPool->QueueJobs(jobs.begin(), jobs.end());

		// Wait for thread to begin executing the jobs
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Ensure running count is as expected while all jobs are running
		ASSERT_EQ(m_threadPool->RunningCount(), jobs.size());
	}

	// IdleCount() test
	// with all threads idle
	TEST_F(ThreadPoolTests, IdleCount_AllIdle)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(2);

		// Ensure idle count is the size of the thread pool
		ASSERT_EQ(m_threadPool->IdleCount(), m_threadPool->Size());
	}

	// IdleCount() test
	// with no threads idle
	TEST_F(ThreadPoolTests, IdleCount_NoneIdle)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(2);

		std::array<std::shared_ptr<TimedJob>, 2> jobs;
		m_jobs.reserve(jobs.size());
		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Create job
			jobs[i] = CreateJob();
		}

		// Enqueue jobs to thread pool
		m_threadPool->QueueJobs(jobs.begin(), jobs.end());

		// Wait for thread to begin executing the jobs
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Ensure idle count is as expected while all jobs are running
		ASSERT_EQ(m_threadPool->IdleCount(), 0);
	}

	// IdleCount() test
	// with half threads idle
	TEST_F(ThreadPoolTests, IdleCount_HalfIdle)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(6);

		std::array<std::shared_ptr<TimedJob>, 3> jobs;
		m_jobs.reserve(jobs.size());
		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Create job
			jobs[i] = CreateJob();
		}

		// Enqueue jobs to thread pool
		m_threadPool->QueueJobs(jobs.begin(), jobs.end());

		// Wait for thread to begin executing the jobs
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Ensure idle count is as expected while all jobs are running
		ASSERT_EQ(m_threadPool->IdleCount(), jobs.size());
	}

	// PendingJobsCount() test
	TEST_F(ThreadPoolTests, PendingJobsCount)
	{
		// Create thread pool
		m_threadPool = std::make_unique<ThreadPool>(4);

		std::array<std::shared_ptr<TimedJob>, 8> jobs;
		m_jobs.reserve(jobs.size());
		for (size_t i = 0; i < jobs.size(); i++)
		{
			// Create job
			jobs[i] = CreateJob();
		}

		// Enqueue jobs to thread pool
		m_threadPool->QueueJobs(jobs.begin(), jobs.end());

		// Wait for thread to begin executing the jobs
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Ensure pending jobs are the number of queued jobs - thread pool size
		// (since some jobs are being executed while some are pending to be executed)
		ASSERT_EQ(m_threadPool->PendingJobsCount(), jobs.size() - static_cast<size_t>(m_threadPool->Size()));
	}
}
