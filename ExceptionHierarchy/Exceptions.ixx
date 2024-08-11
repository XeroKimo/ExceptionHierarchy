module;

#include <concepts>

export module xk.Exceptions;

namespace xk
{
	export struct ExceptionModuleTag {};
	export struct ErrorTag {};

	export template<class Ty>
	concept ExceptionModule = std::same_as<typename Ty::exception_tag, ExceptionModuleTag>;

	export template<class Ty>
	concept Error = std::same_as<typename Ty::exception_tag, ErrorTag>;

	export template<class... Ty>
	class ExceptionImpl;

	export class UnknownException
	{
	public:
		using exception_tag = ErrorTag;
	};

	template<>
	class ExceptionImpl<UnknownException> 
	{

	};

	template<ExceptionModule F>
	class ExceptionImpl<F, UnknownException> :
		public virtual ExceptionImpl<F>,
		public virtual ExceptionImpl<UnknownException>
	{
	public:
		ExceptionImpl() = default;

		template<class... Params>
		ExceptionImpl(Params&&... params) :
			ExceptionImpl<F>{ std::forward<Params>(params)... }
		{

		}
		ExceptionImpl(ExceptionImpl<F> f) :
			ExceptionImpl<F>{ f }
		{

		}
	};

	template<ExceptionModule F, class... Ty>
		requires ((!ExceptionModule<Ty> && !Error<Ty>) && ...)
	class ExceptionImpl<F, UnknownException, Ty...> :
		public ExceptionImpl<F, UnknownException>,
		public ExceptionImpl<F, Ty>...
	{
	public:
		ExceptionImpl() = default;
		ExceptionImpl(ExceptionImpl<F> f, ExceptionImpl<Ty>... t) :
			ExceptionImpl<F>{ f },
			ExceptionImpl<Ty>{ t }...
		{

		}
	};

	template<class Ty>
	class ExceptionImpl<Ty> : 
		public virtual ExceptionImpl<UnknownException>
	{

	};


	template<ExceptionModule F, Error E>
	class ExceptionImpl<F, E> : 
		public virtual ExceptionImpl<F>, 
		public virtual ExceptionImpl<E>,
		public ExceptionImpl<F, UnknownException>
	{
	public:
		ExceptionImpl() = default;
		ExceptionImpl(ExceptionImpl<F> f) :
			ExceptionImpl<F>{ f }
		{

		}

		ExceptionImpl(ExceptionImpl<E> e) :
			ExceptionImpl<E>{ e }
		{

		}

		ExceptionImpl(ExceptionImpl<F> f, ExceptionImpl<E> e) :
			ExceptionImpl<F>{ f },
			ExceptionImpl<E>{ e }
		{

		}
	};

	template<ExceptionModule F, class Ty>
		requires (!ExceptionModule<Ty> && !Error<Ty>)
	class ExceptionImpl<F, Ty> :
		public virtual ExceptionImpl<F>,
		public virtual ExceptionImpl<Ty>
	{

	public:
		ExceptionImpl() = default;
		ExceptionImpl(ExceptionImpl<F> f) :
			ExceptionImpl<F>{ f }
		{

		}

		ExceptionImpl(ExceptionImpl<Ty> t) :
			ExceptionImpl<Ty>{ t }
		{

		}

		ExceptionImpl(ExceptionImpl<F> f, ExceptionImpl<Ty> t) :
			ExceptionImpl<F>{ f },
			ExceptionImpl<Ty>{ t }
		{

		}
	};

	template<Error E, class Ty>
		requires (!ExceptionModule<Ty> && !Error<Ty> && !std::same_as<E, UnknownException>)
	class ExceptionImpl<E, Ty> :
		public virtual ExceptionImpl<E>,
		public virtual ExceptionImpl<Ty>
	{
	public:
		ExceptionImpl() = default;
		ExceptionImpl(ExceptionImpl<E> e) :
			ExceptionImpl<E>{ e }
		{

		}

		ExceptionImpl(ExceptionImpl<Ty> t) :
			ExceptionImpl<Ty>{ t }
		{

		}

		ExceptionImpl(ExceptionImpl<E> e, ExceptionImpl<Ty> t) :
			ExceptionImpl<E>{ e },
			ExceptionImpl<Ty>{ t }
		{

		}
	};

	template<ExceptionModule F, Error E, class... Ty>
		requires ((!ExceptionModule<Ty> && !Error<Ty>) && ...)
	class ExceptionImpl<F, E, Ty...> :
		public ExceptionImpl<F, E>,
		public ExceptionImpl<F, Ty>..., 
		public ExceptionImpl<E, Ty>...
	{

	};

	//Used to allow re-arranging arguments to mean the exact same thing
	//Requires specializing
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

	export template<class Ty, class Invocable, class Invocable2>
		requires std::is_invocable_r_v<Ty, Invocable>
	Ty ConvertTo(Invocable i, Invocable2 i2)
	{
		try
		{
			return i();
		}
		catch(...)
		{
			throw i2();
		}
	}

	export template<class Exception, class Ty, class Invocable, std::invocable<Exception> Invocable2>
		requires std::is_invocable_r_v<Ty, Invocable>
	Ty ConvertTo(Invocable i, Invocable2 i2)
	{
		try
		{
			return i();
		}
		catch(const Exception& e)
		{
			throw i2(e);
		}
	}


}