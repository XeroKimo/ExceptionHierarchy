module;

#include <concepts>
#include <string_view>
#include <string>

export module xk.Exceptions;

namespace xk
{
	export struct ErrorModuleTag {};
	export struct ErrorTag {};
	export struct ErrorCategoryTag {};

	export template<class Ty>
	concept ErrorModule = std::same_as<typename Ty::exception_tag, ErrorModuleTag>;

	export template<class Ty>
	concept Error = std::same_as<typename Ty::exception_tag, ErrorTag>;

	export template<class Ty>
	concept ErrorCategory = std::same_as<typename Ty::exception_tag, ErrorCategoryTag>;

	export template<class... Ty>
	struct ExceptionData;

	template<ErrorModule F>
	struct ExceptionData<F> { std::string_view What() const noexcept { return ""; } };

	template<Error E>
	struct ExceptionData<E> { std::string_view What() const noexcept { return ""; } };

	template<ErrorCategory C>
	struct ExceptionData<C> { std::string_view What() const noexcept { return ""; } };

	template<ErrorModule F, Error E>
	struct ExceptionData<F, E> { std::string_view What() const noexcept { return ""; } };

	template<ErrorModule F, ErrorCategory C>
	struct ExceptionData<F, C> { std::string_view What() const noexcept { return ""; } };

	template<Error E, ErrorCategory C>
	struct ExceptionData<E, C> { std::string_view What() const noexcept { return ""; } };

	template<ErrorModule F, Error E, ErrorCategory C>
	struct ExceptionData<F, E, C> { std::string_view What() const noexcept { return ""; } };

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

	public:
		virtual std::string_view What() const noexcept { return ""; }
		virtual std::string FullWhat() const noexcept { return std::string{ What() }; }
	};

	template<ErrorModule F>
	class ExceptionImpl<F> :
		public virtual ExceptionImpl<UnknownException>
	{
		[[msvc::no_unique_address]] ExceptionData<F> data;
	public:
		ExceptionImpl() = default;

		ExceptionImpl(ExceptionData<F> data) :
			data{ std::move(data)... }
		{

		}

		const ExceptionData<F>& Data() const { return data; }

		virtual std::string_view What() const noexcept { return data.What(); }
		virtual std::string FullWhat() const noexcept { return std::string{ ExceptionImpl::What() }; }
	};

	template<Error E>
	class ExceptionImpl<E> :
		public virtual ExceptionImpl<UnknownException>
	{
		[[msvc::no_unique_address]] ExceptionData<E> data;
	public:
		ExceptionImpl() = default;

		ExceptionImpl(ExceptionData<E> data) :
			data{ std::move(data)... }
		{

		}

		const ExceptionData<E>& Data() const { return data; }

		virtual std::string_view What() const noexcept { return data.What(); }
		virtual std::string FullWhat() const noexcept { return std::string{ ExceptionImpl::What() }; }
	};

	template<ErrorCategory C>
	class ExceptionImpl<C> :
		public virtual ExceptionImpl<UnknownException>
	{
		[[msvc::no_unique_address]] ExceptionData<C> data;
	public:
		ExceptionImpl() = default;

		ExceptionImpl(ExceptionData<C> data) :
			data{ std::move(data)... }
		{

		}

		const ExceptionData<C>& Data() const { return data; }

		virtual std::string_view What() const noexcept { return data.What(); }
		virtual std::string FullWhat() const noexcept { return std::string{ ExceptionImpl::What() }; }
	};

	template<ErrorModule F, Error E>
		requires (!std::same_as<E, UnknownException>)
	class ExceptionImpl<F, E> :
		public virtual ExceptionImpl<F>,
		public virtual ExceptionImpl<E>
	{
		[[msvc::no_unique_address]] ExceptionData<F, E> data;
	public:
		ExceptionImpl() = default;

		ExceptionImpl(ExceptionData<F, E> data) :
			data{ std::move(data)... }
		{

		}

		ExceptionImpl(ExceptionData<F> data0, ExceptionData<E> data1, ExceptionData<F, E> data) :
			ExceptionImpl<F>{ std::move(data0) },
			ExceptionImpl<E>{ std::move(data1) },
			data{ std::move(data)... }
		{

		}

		const ExceptionData<F, E>& Data() const { return data; }

		virtual std::string_view What() const noexcept { return data.What(); }
		virtual std::string FullWhat() const noexcept { return std::string{ ExceptionImpl::What() } + "\n" + ExceptionImpl<F>::FullWhat() + "\n" + ExceptionImpl<E>::FullWhat(); }
	};

	template<ErrorModule F, ErrorCategory C>
	class ExceptionImpl<F, C> :
		public virtual ExceptionImpl<F>,
		public virtual ExceptionImpl<C>
	{
		[[msvc::no_unique_address]] ExceptionData<F, C> data;
	public:
		ExceptionImpl() = default;

		ExceptionImpl(ExceptionData<F, C> data) :
			data{ std::move(data)... }
		{

		}

		ExceptionImpl(ExceptionData<F> data0, ExceptionData<C> data1, ExceptionData<F, C> data) :
			ExceptionImpl<F>{ std::move(data0) },
			ExceptionImpl<C>{ std::move(data1) },
			data{ std::move(data)... }
		{

		}

		const ExceptionData<F, C>& Data() const { return data; }
		virtual std::string_view What() const noexcept { return data.What(); }
		virtual std::string FullWhat() const noexcept { return std::string{ ExceptionImpl::What() } + "\n" + ExceptionImpl<F>::FullWhat() + "\n" + ExceptionImpl<C>::FullWhat(); }
	};

	template<Error E, ErrorCategory C>
	class ExceptionImpl<E, C> :
		public virtual ExceptionImpl<E>,
		public virtual ExceptionImpl<C>
	{
		[[msvc::no_unique_address]] ExceptionData<E, C> data;
	public:
		ExceptionImpl() = default;

		ExceptionImpl(ExceptionData<E, C> data) :
			data{ std::move(data)... }
		{

		}

		ExceptionImpl(ExceptionData<E> data0, ExceptionData<C> data1, ExceptionData<E, C> data) :
			ExceptionImpl<E>{ std::move(data0) },
			ExceptionImpl<C>{ std::move(data1) },
			data{ std::move(data)... }
		{

		}

		const ExceptionData<E, C>& Data() const { return data; }
		virtual std::string_view What() const noexcept { return data.What(); }
		virtual std::string FullWhat() const noexcept { return std::string{ ExceptionImpl::What() } + "\n" + ExceptionImpl<E>::FullWhat() + "\n" + ExceptionImpl<C>::FullWhat(); }
	};

	template<ErrorModule F, Error E, ErrorCategory C>
		requires (!std::same_as<E, UnknownException>)
	class ExceptionImpl<F, E, C> :
		public ExceptionImpl<F, E>,
		public ExceptionImpl<F, C>,
		public ExceptionImpl<E, C>
	{
		[[msvc::no_unique_address]] ExceptionData<F, E, C> data;
	public:
		ExceptionImpl() = default;

		ExceptionImpl(ExceptionData<F, E, C> data) :
			data{ std::move(data)... }
		{

		}

		ExceptionImpl(ExceptionData<F, E> data0, ExceptionData<F, C> data1, ExceptionData<E, C> data2, ExceptionData<F, E, C> data) :
			ExceptionImpl<F, E>{ std::move(data0) },
			ExceptionImpl<F, C>{ std::move(data1) },
			ExceptionImpl<E, C>{ std::move(data1) },
			data{ std::move(data)... }
		{

		}

		ExceptionImpl(ExceptionData<F> data0, ExceptionData<E> data1, ExceptionData<E> data2, ExceptionData<F, E> data3, ExceptionData<F, C> data4, ExceptionData<E, C> data5, ExceptionData<F, E, C> data) :
			ExceptionImpl<F>{ std::move(data0) },
			ExceptionImpl<E>{ std::move(data1) },
			ExceptionImpl<C>{ std::move(data2) },
			ExceptionImpl<F, E>{ std::move(data3) },
			ExceptionImpl<F, C>{ std::move(data4) },
			ExceptionImpl<E, C>{ std::move(data5) },
			data{ std::move(data)... }
		{

		}

		const ExceptionData<F, E, C>& Data() const { return data; }
		virtual std::string_view What() const noexcept { return data.What(); }
		virtual std::string FullWhat() const noexcept 
		{ 
			return std::string{ ExceptionImpl::What() } + "\n" + 
				std::string{ ExceptionImpl<F, E>::What() } + "\n" + 
				std::string{ ExceptionImpl<F, C>::What() } + "\n" +
				std::string{ ExceptionImpl<E, C>::What() } + "\n" +
				std::string{ ExceptionImpl<F>::What() } + "\n" +
				std::string{ ExceptionImpl<E>::What() } + "\n" +
				std::string{ ExceptionImpl<C>::What() };
		}
	};

	//Used to allow re-arranging arguments to mean the exact same thing
	//Requires specializing
	export template<class... Ty>
	struct ExceptionAliasMap
	{
		using type = ExceptionImpl<Ty...>;
	};

	template<Error E, ErrorModule F>
	struct ExceptionAliasMap<E, F>
	{
		using type = ExceptionImpl<F, E>;
	};

	template<ErrorCategory C, ErrorModule F>
	struct ExceptionAliasMap<C, F>
	{
		using type = ExceptionImpl<F, C>;
	};

	template<ErrorCategory C, Error E>
	struct ExceptionAliasMap<E, C>
	{
		using type = ExceptionImpl<E, C>;
	};

	template<ErrorModule F, ErrorCategory C, Error E>
	struct ExceptionAliasMap<F, C, E>
	{
		using type = ExceptionImpl<F, E, C>;
	};

	template<Error E, ErrorCategory C, ErrorModule F>
	struct ExceptionAliasMap<E, C, F>
	{
		using type = ExceptionImpl<F, E, C>;
	};

	template<Error E, ErrorModule F, ErrorCategory C>
	struct ExceptionAliasMap<E, F, C>
	{
		using type = ExceptionImpl<F, E, C>;
	};

	template<ErrorCategory C, Error E, ErrorModule F>
	struct ExceptionAliasMap<C, E, F>
	{
		using type = ExceptionImpl<F, E, C>;
	};

	template<ErrorCategory C, ErrorModule F, Error E>
	struct ExceptionAliasMap<C, F, E>
	{
		using type = ExceptionImpl<F, E, C>;
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