#include "aton/range/adaptors/transform.hpp"

#include "boost/test/unit_test.hpp"

#include <vector>


BOOST_AUTO_TEST_SUITE(adaptors)

BOOST_AUTO_TEST_SUITE(transform)

BOOST_AUTO_TEST_SUITE(smoke)

auto square = std::function<int (int)>{ [](int i) { return i*i; } };


BOOST_AUTO_TEST_CASE(minimal)
{
	std::vector<int> range {1, 2, 3};
	int expected = 1;
	for (int i : range | aton::adaptors::transform(square))
	{
		BOOST_CHECK_EQUAL(i, square(expected));
		++expected;
	}
}


struct SurvivalChecker : std::vector<int>
{
	bool *survived;

	~SurvivalChecker() { if (survived) *survived = false; }

	SurvivalChecker(SurvivalChecker &&src) ATON_NOEXCEPT :
		survived(src.survived)
	{ src.survived = nullptr; }

	SurvivalChecker& operator= (SurvivalChecker &&src) ATON_NOEXCEPT
	{
		if (this == &src) {
			return *this;
		}
		survived = src.survived;
		src.survived = nullptr;
		return *this;
	}

	explicit SurvivalChecker(bool &survived) : survived(&survived) {}
};


BOOST_AUTO_TEST_CASE(transform_rvalue_survival_boost)
{
	bool survived = true;
	auto &&rvalue = SurvivalChecker(survived) | boost::adaptors::transformed(square);
	BOOST_CHECK(!survived);
}


BOOST_AUTO_TEST_CASE(transform_rvalue_survival)
{
	bool survived = true;
	auto &&rvalue = SurvivalChecker(survived) | aton::adaptors::transform(square);
	BOOST_CHECK(survived);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
