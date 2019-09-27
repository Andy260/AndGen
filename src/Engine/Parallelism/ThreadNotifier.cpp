#include "ThreadNotifier.hpp"

// AndGen includes
#include <AndGen/Exceptions/NotImplementedException.hpp>

// Notifies all waiting threads
void AndGen::ThreadNotifier::Notify()
{
	m_shouldWake = true;
	m_conditionVariable.notify_all();
}

// Blocks the calling thread until another thread notifies to wake up
void AndGen::ThreadNotifier::Wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_conditionVariable.wait(lock, [this] { return m_shouldWake.load(); });
}
