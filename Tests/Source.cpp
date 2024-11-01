#include <iostream>
#include <concepts>
import xk.Exceptions;

using namespace xk;


class TestModule
{
	using exception_tag = ErrorModuleTag;
};

class TestError
{
	using exception_tag = ErrorTag;
};

struct TestCategory
{
	using exception_tag = ErrorCategoryTag;
};

template<>
struct ExceptionData<TestModule>
{
	std::string_view What() const noexcept { return "Test Module"; }
};

template<>
struct ExceptionData<TestError>
{
	std::string_view What() const noexcept { return "Test Error"; }
};

template<>
struct ExceptionData<TestCategory>
{
	std::string_view What() const noexcept { return "Test Category"; }
};

template<>
struct ExceptionData<TestModule, TestError>
{
	std::string_view What() const noexcept { return "Test Module + Error"; }
};

template<>
struct ExceptionData<TestError, TestCategory>
{
	std::string_view What() const noexcept { return "Test Error + Category"; }
};

template<>
struct ExceptionData<TestModule, TestCategory>
{
	std::string_view What() const noexcept { return "Test Module + Category"; }
};

template<>
struct ExceptionData<TestModule, TestError, TestCategory>
{
	std::string_view What() const noexcept { return "Test Module + Error + Category"; }
};

int main()
{
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestModule>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestError>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestCategory>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestModule, TestError>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestModule, TestCategory>&>);
	static_assert(std::convertible_to<Exception<TestModule, TestError, TestCategory>&, Exception<TestError, TestCategory>&>);

	Exception<TestModule, TestError, TestCategory> e;
	Exception<UnknownException>& e2 = e;
	std::cout << e2.FullWhat();

	ExceptionData<TestModule, TestError, TestCategory> d = e.Data();
	ExceptionData<TestModule, TestError> d2 = e.Exception<TestModule, TestError>::Data();
	ExceptionData<TestModule> d3 = e.Exception<TestModule>::Data();

}