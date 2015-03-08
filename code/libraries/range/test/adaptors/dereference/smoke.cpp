#include "aton/range/adaptors/dereference.hpp"

#include "boost/test/unit_test.hpp"

#include <memory>
#include <vector>


BOOST_AUTO_TEST_SUITE(adaptors)

BOOST_AUTO_TEST_SUITE(dereference)

BOOST_AUTO_TEST_SUITE(smoke)

BOOST_AUTO_TEST_CASE(minimal)
{
	std::vector<std::shared_ptr<int>> range {
		std::make_shared<int>(1),
		std::make_shared<int>(2),
		std::make_shared<int>(3)
	};
	std::vector<int> result;
	for (int i : range | aton::adaptors::dereference) {
		result.push_back(i);
	}
	BOOST_REQUIRE_EQUAL(result.size(), 3);
	BOOST_CHECK_EQUAL(result[0], 1);
	BOOST_CHECK_EQUAL(result[1], 2);
	BOOST_CHECK_EQUAL(result[2], 3);
}


BOOST_AUTO_TEST_CASE(write)
{
	std::vector<std::shared_ptr<int>> range {
		std::make_shared<int>(1),
		std::make_shared<int>(2),
		std::make_shared<int>(3)
	};
	for (int& i : range | aton::adaptors::dereference) {
		i *= 2;
	}
	BOOST_CHECK_EQUAL(*range[0], 2);
	BOOST_CHECK_EQUAL(*range[1], 4);
	BOOST_CHECK_EQUAL(*range[2], 6);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
