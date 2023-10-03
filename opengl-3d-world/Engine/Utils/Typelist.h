#pragma once

template<typename ...Types>
struct TypeList {};

template<typename List>
struct Front;
template<typename First, typename ...Types>
struct Front<TypeList<First, Types...>>
{
	using type = First;
};

template<typename List>
struct PopFront;
template<typename First, typename ...Types>
struct PopFront<TypeList<First, Types...>>
{
	using type = TypeList<Types...>;
};

template<typename List, typename Type>
struct PushFront;
template<typename ...Types, typename Type>
struct PushFront<TypeList<Types...>, Type>
{
	using type = TypeList<Type, Types...>;
};

template<typename List, typename Type>
struct PushBack;
template<typename ...Types, typename Type>
struct PushBack<TypeList<Types...>, Type>
{
	using type = TypeList<Types..., Type>;
};

template<typename List, typename Type>
struct ReplaceFront;
template<typename ...Types, typename Type>
struct ReplaceFront<TypeList<Types...>, Type>
{
	using RemoveFront = typename PopFront<TypeList<Types...>>::type;
	using PushFront = typename PushFront<RemoveFront, Type>::type;
	using type = PushFront;
};

template<typename List, size_t Index>
struct GetAt : GetAt<typename PopFront<List>::type, Index - 1> {};
template<typename List>
struct GetAt<List, 0> : Front<List> {};

template<typename List>
struct Length;
template<typename ...Types>
struct Length<TypeList<Types...>>
{
	static constexpr size_t value = sizeof...(Types);
};

template<bool IsTrue, typename TrueType, typename FalseType>
struct IfThenElse {};
template<typename TrueType, typename FalseType>
struct IfThenElse<true, TrueType, FalseType>
{
	using type = TrueType;
};
template<typename TrueType, typename FalseType>
struct IfThenElse<false, TrueType, FalseType>
{
	using type = FalseType;
};

template<typename List>
struct GetLargest
{
private:
	using First = typename Front<List>::type;
	using Rest = typename GetLargest<typename PopFront<List>::type>::type;
public:
	using type = typename IfThenElse<sizeof(First) >= sizeof(Rest), First, Rest>::type;
};
template<>
struct GetLargest<TypeList<>>
{
	using type = char;
};

template<typename List>
struct IsEmpty
{
	static constexpr bool value = false;
};
template<>
struct IsEmpty<TypeList<>>
{
	static constexpr bool value = true;
};

template<typename List>
struct Back;
template<typename ...Types>
struct Back<TypeList<Types...>>
{
	using type = typename GetAt<TypeList<Types...>, sizeof...(Types) - 1>::type;
};


template<std::size_t CountT, typename T, typename R>
struct MoveItem
{
private:
	using first = typename PushBack<T, typename Front<R>::type>::type;
	using last = typename PopFront<R>::type;

public:
	using type = typename MoveItem<CountT - 1, first, last>::type;
	using rest = typename MoveItem<CountT - 1, first, last>::rest;
};
template<typename T, typename R>
struct MoveItem<0, T, R>
{
	using type = T;
	using rest = R;
};

template<std::size_t PivotT, typename T>
struct Split
{
	static_assert(PivotT <= Length<T>::value,
		"The split pivot index is out of range");

	using type = typename MoveItem<PivotT, TypeList<>, T>::type;
	using rest = typename MoveItem<PivotT, TypeList<>, T>::rest;
};

template<typename List>
struct PopBack
{
	using type = typename Split<Length<List>::value - 1, List>::type;
};
template<>
struct PopBack<TypeList<>>
{
	using type = TypeList<>;
};

template<typename LhsType, typename RhsType>
struct IsSame
{
	static constexpr bool value = false;
};
template<typename LhsType>
struct IsSame<LhsType, LhsType>
{
	static constexpr bool value = true;
};

template<typename Type, Type Value>
struct IntegralConstant
{
	using type = Type;
	static constexpr Type value = Value;
};

template<typename List, typename Type, bool IsListEmpty = IsEmpty<List>::value>
struct Contains;
template<typename List, typename Type>
struct Contains<List, Type, true> : IntegralConstant<bool, false> {};
template<typename List, typename Type>
struct Contains<List, Type, false> : IfThenElse<
	IsSame<typename Front<List>::type, Type>::value,
	IntegralConstant<bool, true>,
	Contains<typename PopFront<List>::type, Type>>::type{};
