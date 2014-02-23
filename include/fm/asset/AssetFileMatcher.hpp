#ifndef HEADER_ZN_ASSETFILEMATCHER_HPP_INCLUDED
#define HEADER_ZN_ASSETFILEMATCHER_HPP_INCLUDED

#include <string>
#include <vector>
#include <regex>

#include <fm/json/json_utils.hpp>

namespace zn
{

/// \brief A simple filename matcher.
/// It constructs an ECMAScript-style regex from simplified patterns.
/// The matching process goes through every pattern, and succeeds if at least one matches.
/// \note Patterns can be expressed as a single regex. However, it's not always convenient to
/// read, then the pattern list is implemented for convenience.
class AssetFileMatcher
{
public:

	/// \brief Adds a regex pattern to match.
	/// \param simplePattern: the pattern to add.
	/// '.' and '*' are respectively interpreted as '\.' and '.+'
	void addPattern(std::string simplePattern);

	/// \brief tests if the given string matches one of the patterns.
	/// \param str: file path to test
	/// \return true if at least one pattern matches, false otherwise.
	bool evaluate(const std::string & str);

	/// \brief Loads patterns from a JSON array of strings.
	/// Does nothing if the array is empty.
	/// Empty strings or non-string elements are ignored.
	/// \param obj: JSON array.
	void loadFromJSON(JsonBox::Value obj);

private:

	std::vector<std::regex> m_patterns;

};

} // namespace zn

#endif // HEADER_ZN_ASSETFILEMATCHER_HPP_INCLUDED
