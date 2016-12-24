#define BOOST_TEST_MODULE Simplify
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/operations.hpp>

#include "filter.h"
#define XSTR(s) STR(s)
#define STR(s) #s
boost::filesystem::path root(XSTR(ROOT));

using namespace Simplify;

std::set<std::string>
readlines(const boost::filesystem::path & input)
{
	std::set<std::string> out;
	std::ifstream s(input.string());
	BOOST_TEST_INFO(input);
	BOOST_REQUIRE(s.good());
	std::string line;
	while (!std::getline(s, line).eof()) {
		out.insert(line);
	}
	return out;
}

namespace std {
	template<typename T>
	std::ostream & operator<<(std::ostream & strm, const std::set<T> & set)
	{
		std::copy(set.begin(), set.end(), std::ostream_iterator<T>(strm, "\n"));
		return strm;
	}
}

BOOST_FIXTURE_TEST_SUITE( filter, Filter );

BOOST_AUTO_TEST_CASE( defaultOptions )
{
	FilterOptions fo;
	initiailize(fo);
	BOOST_REQUIRE(exclude.empty());
	BOOST_REQUIRE(accepted.empty());
}

BOOST_AUTO_TEST_CASE( addMountpoints )
{
	FilterOptions fo;
	fo.excludeMounts = true;
	initiailize(fo);
	BOOST_REQUIRE(!exclude.empty());
	BOOST_REQUIRE(std::find(exclude.begin(), exclude.end(), "/") != exclude.end());
	BOOST_REQUIRE(std::find(exclude.begin(), exclude.end(), "/tmp") != exclude.end());
	BOOST_REQUIRE(std::find(exclude.begin(), exclude.end(), "/etc") == exclude.end());
}

BOOST_AUTO_TEST_CASE( prefixing )
{
	const Filter::Path p1("/");
	const Filter::Path p2("/bin");
	const Filter::Path p3("/bin/bash");
	const Filter::Path p4("/boot");
	const Filter::Path p5("/boot/grub");

	BOOST_REQUIRE_EQUAL(true, boost::algorithm::starts_with(p2, p1));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p1, p2));
	BOOST_REQUIRE_EQUAL(true, boost::algorithm::starts_with(p3, p1));
	BOOST_REQUIRE_EQUAL(true, boost::algorithm::starts_with(p3, p2));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p2, p3));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p1, p3));
	BOOST_REQUIRE_EQUAL(true, boost::algorithm::starts_with(p5, p1));
	BOOST_REQUIRE_EQUAL(true, boost::algorithm::starts_with(p5, p4));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p4, p5));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p1, p5));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p2, p4));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p2, p4));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p2, p5));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p5, p2));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p3, p4));
	BOOST_REQUIRE_EQUAL(false, boost::algorithm::starts_with(p4, p3));
}

BOOST_AUTO_TEST_CASE( simpleRoot )
{
	FilterOptions fo;
	initiailize(fo);
	BOOST_REQUIRE_EQUAL(true, accept("/"));
	BOOST_REQUIRE_EQUAL(false, accept("/bin"));
	BOOST_REQUIRE_EQUAL(false, accept("/boot"));
}

BOOST_AUTO_TEST_CASE( simpleDev )
{
	FilterOptions fo;
	initiailize(fo);
	BOOST_REQUIRE_EQUAL(true, accept("/home/randomdan/dev"));
	BOOST_REQUIRE_EQUAL(false, accept("/home/randomdan/dev/bdr-plugin"));
	BOOST_REQUIRE_EQUAL(false, accept("/home/randomdan/dev/arch.dot"));
	BOOST_REQUIRE_EQUAL(false, accept("/home/randomdan/dev/.ycm_extra_conf.py"));
}

BOOST_AUTO_TEST_CASE( simpleStream )
{
	FilterOptions fo;
	initiailize(fo);
	Path output = boost::filesystem::canonical("/proc/self/exe").parent_path() / "output.txt";
	Path input1 = root / "fixtures/input1";
	Path expected1 = root / "fixtures/expected1";

	std::ifstream in(input1.string());
	std::ofstream out(output.string());
	filterStream(in, out);

	BOOST_REQUIRE_EQUAL(readlines(output), readlines(expected1));
}

BOOST_AUTO_TEST_CASE( simpleStreamExclude )
{
	FilterOptions fo;
	fo.excludes.push_back("/bin");
	initiailize(fo);
	Path output = boost::filesystem::canonical("/proc/self/exe").parent_path() / "output.txt";
	Path input1 = root / "fixtures/input1";
	Path expected1 = root / "fixtures/expected1exclude";

	std::ifstream in(input1.string());
	std::ofstream out(output.string());
	filterStream(in, out);

	BOOST_REQUIRE_EQUAL(readlines(output), readlines(expected1));
}

BOOST_AUTO_TEST_CASE( excludesBin )
{
	FilterOptions fo;
	fo.excludes.push_back("/bin");
	initiailize(fo);
	BOOST_REQUIRE_EQUAL(1, exclude.size());
	BOOST_REQUIRE_EQUAL("/bin", exclude.begin()->string());
	BOOST_REQUIRE_EQUAL(false, accept("/"));
	BOOST_REQUIRE_EQUAL(false, accept("/bin"));
	BOOST_REQUIRE_EQUAL(true, accept("/bin/bash"));
	BOOST_REQUIRE_EQUAL(true, accept("/bin/grep"));
	BOOST_REQUIRE_EQUAL(true, accept("/boot"));
	BOOST_REQUIRE_EQUAL(false, accept("/boot/grub"));
}

BOOST_AUTO_TEST_SUITE_END();

