#pragma once
#include <typeindex>
#include <unordered_map>
#include "Event.h"

namespace Engine
{

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define SET_DATA_EVENT_NAME(name) struct CONCAT(DataEventName_, name) { static constexpr const char value[] = #name; }
#define GET_DATA_EVENT_NAME(name) CONCAT(DataEventName_, name)::value

inline std::unordered_map<std::type_index, EventType> type_map = {};

template<typename Type>
class DataEvent : public Event
{
public:
	DataEvent(Type value) : m_value(value) {}

	static EventType getStaticType()
	{
		static const EventType type = generateType();
		return type;
	}

	EventType getEventType() const override
	{
		return getStaticType();
	}

	EVENT_CLASS_CATEGORY(None)
	SET_DATA_EVENT_NAME(Type);

	Type getValue()
	{
		return m_value;
	}

	const char* getName() const override
	{
		return GET_DATA_EVENT_NAME(Type);
	}

private:
	static EventType generateType()
	{
		const std::type_index type_index(typeid(Type));

		const auto it = type_map.find(type_index);
		if (it != type_map.end())
			return it->second;

		const auto new_type = static_cast<EventType>(static_cast<int>(EventType::DataValueStart) + type_map.size() + 1);
		type_map[type_index] = new_type;

		return new_type;
	}

	Type m_value;
};


}
