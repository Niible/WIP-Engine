#pragma once
#include <memory>

#include "TypeList.h"

template<typename ValidTypes, template<class...> class SmartPtrType = std::unique_ptr>
class Factory
{
public:

	template<typename Type>
	using SmartPtr = SmartPtrType<Type>;

	template<typename Type, typename... Args>
	static SmartPtr<Type> create(Args&&... args)
	{
		static_assert(Contains<ValidTypes, Type>::value);
		return SmartPtr<Type>(new Type(std::forward<Args>(args)...));
	}
};

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> createScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> createRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}