#ifndef COMMANDLINEARGUMENTS_H
#define	COMMANDLINEARGUMENTS_H

// STL includes
#include <sstream>
#include <ostream>
#include <vector>
#include <string>
// AndGen includes
#include "AndGen/Engine/Exceptions/NotImplementedException.hpp"

namespace AndGen
{
	/// <summary>
	/// Represents the given command line arguments for this application
	/// </summary>
	class CommandLineArguments
	{
	public:
		/// <summary>
		/// Constructs a new Command Line Arguments object with the given command line arguments
		/// </summary>
		/// <param name="argc">Amount of command line arguments</param>
		/// <param name="argv">Pointer to array of argument strings</param>
		/// <exception cref="std::invalid_argument">
		/// Thrown when either <paramref name="argc"/> is negative, 
		/// or <paramref name="argv"/> is NULL
		/// </exception>
		/// <remarks>
		/// <para>This is the primary constructor for creating Command Line Argument wrapper object. 
		/// Intended for usage by the engine internally.</para>
		/// <para>If <paramref name="argc"/> is 0, then <paramref name="argv"/> is ignored.</para>
		/// </remarks>
		CommandLineArguments(int argc, char* argv[]);

		/// <summary>
		/// Gets all arguments from the command line
		/// </summary>
		inline const std::vector<std::string>& GetAllArguments() const
		{
			return m_arguments;
		}

		/// <summary>
		/// Checks if a given argument exists
		/// </summary>
		/// <param name="argument">Argument to check for</param>
		bool HasArgument(const std::string& argument);

		/// <summary>
		/// Total number of arguments
		/// </summary>
		inline size_t Count() const
		{
			return m_arguments.size();
		}

		/// <summary>
		/// Subscript operator which returns an argument by index
		/// </summary>
		/// <remarks>
		/// The subscript operator will return an argument with a given index (if exists), 
		/// otherwise will throw a std::out_of_range exception.
		/// </remarks>
		/// <param name="index">Index of argument</param>
		/// <exception cref="std::out_of_range">Thrown when index is out of range</exception>
		inline std::string& operator[](size_t index)
		{
			// Ensure index is within range
			if (index < 0 || index >= m_arguments.size())
			{
				throw std::out_of_range("index is out of range");
			}

			return m_arguments[index];
		}

	private:
		// Internal storage of application command line arguments
		std::vector<std::string> m_arguments;
	};
}

#endif
