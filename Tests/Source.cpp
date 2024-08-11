#include <iostream>
#include <concepts>
import xk.Exceptions;

using namespace xk;


class TestModule
{
	using exception_tag = ExceptionModuleTag;
};

class TestError
{
	using exception_tag = ErrorTag;
};

struct TestCategory;


int main()
{
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestModule>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestError>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestCategory>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestModule, TestError>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestModule, TestCategory>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestError, TestCategory>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestModule, UnknownException>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<UnknownException>&>);


	//Exception<TestModule>& e = f;
	//Exception<TestError>& e2 = f;
	//Exception<TestCategory>& e3 = f;

	//Exception<TestModule, TestError>& e4 = f;
	//Exception<TestModule, TestCategory>& e5 = f;
	//Exception<TestError, TestCategory>& e6 = f;

	//Exception<TestModule, UnknownException>& e7 = f;
	//Exception<UnknownException>& e8 = f;
}