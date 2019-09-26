#include "JobQueue.hpp"

// STL includes
#include <queue>
// AndGen includes
#include <AndGen/Engine/Jobs/Job.hpp>
#include <AndGen/Exceptions/NotImplementedException.hpp>

// Adds a job to the end of the queue
void AndGen::JobQueue::AddJob(std::shared_ptr<AndGen::Job> job)
{
	// Ensure we are given a valid job
	if (job == nullptr)
	{
		return;
	}

	// Acquire lock on queue
	std::scoped_lock<std::mutex> lock(m_jobQueue_mutex);
	// Add job to queue
	m_jobQueue.push_back(job);
}

// Adds all jobs for another queue into this queue
void AndGen::JobQueue::AddJobQueue(const AndGen::JobQueue& jobQueue)
{
	// Ignore empty job queues
	if (jobQueue.Count() <= 0)
	{
		return;
	}

	// Acquire lock on queue
	std::scoped_lock<std::mutex> lock(m_jobQueue_mutex);
	// Add given job queue's jobs to this queue
	for (size_t i = 0; i < jobQueue.Count(); i++)
	{
		m_jobQueue.push_back(jobQueue.m_jobQueue[i]);
	}
}

// Executes the next job
void AndGen::JobQueue::ExecuteNextJob()
{
	// Get next job from the queue
	std::shared_ptr<Job> nextJob = GetNextJob();
	// Do nothing if we don't have a job to execute
	if (nextJob == nullptr)
	{
		return;
	}

	// Execute job on main thread
	nextJob->Run();
}

// Gets next job from the queue, if any
std::shared_ptr<AndGen::Job> AndGen::JobQueue::GetNextJob()
{
	// Return null pointer if no jobs are left
	if (m_jobQueue.size() <= 0)
	{
		return nullptr;
	}

	// Acquire lock on queue
	std::scoped_lock<std::mutex> lock(m_jobQueue_mutex);
	// Get pointer to next job and pop it from the queue
	std::shared_ptr<Job> nextJob = m_jobQueue.front();
	m_jobQueue.pop_front();

	return nextJob;
}

// Empties the queue
void AndGen::JobQueue::Clear()
{
	// Acquire lock on queue
	std::scoped_lock<std::mutex> lock(m_jobQueue_mutex);

	// Empty queue
	std::deque<std::shared_ptr<Job>> emptyQueue;
	m_jobQueue.swap(emptyQueue);
}
