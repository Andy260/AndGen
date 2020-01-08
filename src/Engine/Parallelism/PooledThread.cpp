#include "PooledThread.hpp"
#include "PooledThread.hpp"

// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>
#include <AndGen/Exceptions/NotImplementedException.hpp>

// Begins thread execution
void AndGen::PooledThread::Start()
{
	// Ignore call if thread is already running
	if (m_isRunning)
	{
		return;
	}

	m_shouldExit = false;
	// Thread running flag set here, in case threads check immediately if this thread is running
	// before first line of the internal thread gets to run
	m_isRunning	= true;

	// Start thread
	m_thread = std::thread(&PooledThread::ExecutionLoop, this);
}

// Stops thread execution
void AndGen::PooledThread::Stop(bool waitForThread)
{
	// Ignore call if thread isn't running
	if (!m_isRunning)
	{
		return;
	}

	m_shouldExit = true;
	// Wake thread, if waiting for jobs to be added
	m_jobsReadyNotification.Notify();

	// Wait for thread to finish executing
	if (waitForThread && m_thread.joinable())
	{
		m_thread.join();
	}
}

// Waits for current queue to be executed by the thread
void AndGen::PooledThread::WaitForQueue()
{
	// Ignore call if queue is empty
	// or thread isn't running
	if (m_jobQueue.Count() <= 0 ||
		!m_isRunning)
	{
		return;
	}

	m_jobsCompleteNotification.Wait();
}

// Executes all jobs in the queue
// and waits for a job to be added when none are left
void AndGen::PooledThread::ExecutionLoop()
{
	while (!m_shouldExit)
	{
		// Execute the next job in the queue if there's any left
		if (m_jobQueue.Count() > 0)
		{
			m_isExecuting = true;
			m_jobQueue.ExecuteNextJob();
		}

		if (m_jobQueue.Count() <= 0)
		{
			// Notify external waiting threads that the queue is completed
			m_jobsCompleteNotification.Notify();

			// Wait for jobs to be added to the queue
			m_isExecuting = false;
			m_jobsReadyNotification.Wait();
		}
	}

	m_isRunning = false;
}
