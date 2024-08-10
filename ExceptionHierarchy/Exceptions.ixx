module;

#include <concepts>

export module xk.Exceptions;

namespace xk
{
	export template<class Ty>
	constexpr bool IsFrom = false;

	export template<class Ty>
	constexpr bool IsError = false;

	export template<class Ty>
	concept From = IsFrom<Ty> && std::is_class_v<Ty>;

	export template<class Ty>
	concept Error = IsError<Ty> && std::is_class_v<Ty>;

	export template<class... Ty>
		requires (std::is_class_v<Ty> && ...)
	class ExceptionImpl;

	export class UnknownException
	{

	};

	template<>
	constexpr bool IsError<UnknownException> = true;

	export template<class... Ty>
	class ExceptionInterface;

	template<class Ty>
	class ExceptionInterface<Ty>
	{

	};

	template<From F, Error E>
	class ExceptionInterface<F, E> :
		public virtual ExceptionInterface<F>,
		public virtual ExceptionInterface<E>
	{

	};

	template<From F, class Ty>
	class ExceptionInterface<F, Ty> :
		public virtual ExceptionInterface<F>,
		public virtual ExceptionInterface<Ty>
	{

	};

	template<Error E, class Ty>
	class ExceptionInterface<E, Ty> :
		public virtual ExceptionInterface<E>,
		public virtual ExceptionInterface<Ty>
	{

	};

	template<class Ty>
	class ExceptionImpl<Ty> : 
		public virtual Exception<UnknownException>,
		public virtual ExceptionInterface<Ty>
	{

	};

	template<From F, Error E>
	class ExceptionImpl<F, E> : 
		public virtual ExceptionImpl<F>, 
		public virtual ExceptionImpl<E>,
		public ExceptionImpl<F, UnknownException>,
		public ExceptionInterface<F, E>
	{

	};

	template<From F, class Ty>
	class ExceptionImpl<F, Ty> :
		public virtual ExceptionImpl<F>,
		public virtual ExceptionImpl<Ty>,
		public ExceptionInterface<F, Ty>
	{

	};

	template<Error E, class Ty>
	class ExceptionImpl<E, Ty> :
		public virtual ExceptionImpl<E>,
		public virtual ExceptionImpl<Ty>,
		public ExceptionImpl<UnknownException, Ty>,
		public ExceptionInterface<E, Ty>
	{

	};


	template<>
	class ExceptionImpl<UnknownException> :
		public virtual ExceptionInterface<UnknownException>
	{

	};

	template<From F>
	class ExceptionImpl<F, UnknownException> :
		public virtual ExceptionImpl<F>,
		public virtual ExceptionImpl<UnknownException>,
		public ExceptionInterface<F, UnknownException>
	{

	};

	template<class Ty>
	class ExceptionImpl<UnknownException, Ty> :
		public virtual ExceptionImpl<UnknownException>,
		public virtual ExceptionImpl<Ty>,
		public ExceptionInterface<UnknownException, Ty>
	{

	};

	template<From F, Error E, class... Ty>
	class ExceptionImpl<F, E, Ty...> :
		public ExceptionImpl<F, E>,
		public ExceptionImpl<F, Ty>..., 
		public ExceptionImpl<E, Ty>...,
	{

	};

	export template<class... Ty>
	struct ExceptionAliasMap
	{
		using type = ExceptionImpl<Ty...>;
	};

	export template<class... Ty>
	using Exception = ExceptionAliasMap<Ty...>::type;

	export template<class Ty, class Invocable>
		requires std::is_invocable_r_v<Ty, Invocable>
	Ty ValueOr(Invocable i, Ty v)
	{
		try
		{
			return i();
		}
		catch(...)
		{
			return v;
		}
	}


	export template<class Ty, class Invocable, class Invocable2>
		requires std::is_invocable_r_v<Ty, Invocable> && std::is_invocable_r_v<Ty, Invocable2>
	Ty ValueOr(Invocable i, Invocable2 i2)
	{
		try
		{
			return i();
		}
		catch(...)
		{
			return i2();
		}
	}

	class Test
	{
	};
	class TestE
	{
	};

	template<>
	constexpr bool IsFrom<Test> = true;

	template<>
	constexpr bool IsError<TestE> = true;

	struct bafa
	{
		int a;
	};

	
	void Foo()
	{
		ExceptionImpl<Test, TestE, bafa> f;
		Exception<Test>& e = f;
		Exception<TestE>& e2 = f;
		Exception<Test, UnknownException>& e3 = f;
		Exception<UnknownException>& e4 = f;

		Exception<Test, bafa>& e5 = f;
		Exception<TestE, bafa>& e6 = f;
		Exception<UnknownException, bafa>& e7 = f;
		bafa s;
	}
}