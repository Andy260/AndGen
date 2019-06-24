// STL Includes
#include <stdexcept>
// AndGen Engine includes
#include "AndGen/Engine/CommandLineArguments.hpp"
#include "AndGen/Engine/Exceptions/NotImplementedException.hpp"

AndGen::CommandLineArguments::CommandLineArguments(int argc, char* argv[])
{
	// Ensure we were given a valid argument count value
	if (argc < 0)
	{
		throw std::invalid_argument("argc cannot be less than 0");
	}
	else if (argc == 0)
	{
		// No more set-up needed for no arguments
		return;
	}

	// Ensure we were given a valid argument value pointer
	if (argv == nullptr)
	{
		throw std::invalid_argument("argv cannot be NULL");
	}

	// Convert arguments given to application to C++ style strings
	// and store them in this object
	for (size_t i = 0; i < argc; i++)
	{
		// Ensure current argument isn't NULL
		if (argv[i] == nullptr)
		{
			continue;
		}

		m_arguments.push_back(std::string(argv[i]));
	}
}

bool AndGen::CommandLineArguments::HasArgument(const std::string& argument)
{
	// Check if any of the strings within the saved arguments are equal to the given argument
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		// std::string::compare() returns 0 if strings are equal
		if (m_arguments[i].compare(argument) == 0)
		{
			return true;
		}
	}

	return false;
}
