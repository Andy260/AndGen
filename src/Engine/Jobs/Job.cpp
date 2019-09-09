#include <AndGen/Engine/Jobs/Job.hpp>

// AndGen includes
#include <AndGen/Exceptions/NotImplementedException.hpp>

void AndGen::Job::Internal_Schedule(std::shared_ptr<AndGen::Job> dependsOn)
{
	throw NotImplementedException();
}
