#pragma once
#include <string>

#include <iostream>

namespace Engine
{

enum class EventType
{
	None = 0,
	WindowClose, WindowResize,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	DataValue,
	DataValueStart = 1000, // Start of DataValue event types
};

enum EventCategory
{
	None = 0x0,
	EventCategoryApplication = 0x1,
	EventCategoryInput = 0x2,
	EventCategoryKeyboard = 0x4,
	EventCategoryMouse = 0x8,
	EventCategoryMouseButton = 0x10
};


class Event
{
public:
	virtual ~Event() = default;

	virtual EventType getEventType() const = 0;
	virtual const char* getName() const = 0;
	virtual int getCategoryFlags() const = 0;
	virtual std::string toString() const { return getName(); }

	bool isInCategory(const EventCategory category) const
	{
		return getCategoryFlags() & category;
	}

	bool handled = false;
};

#define EVENT_CLASS_TYPE(type)\
static EventType getStaticType() { return EventType::type; } \
virtual EventType getEventType() const override { return getStaticType(); }\

#define EVENT_CLASS_NAME(type)\
virtual const char* getName() const override { return #type; }\

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

class EventDispatcher
{
public:
	explicit EventDispatcher(Event& event)
		: m_event(event)
	{
	}

	// F will be deduced by the compiler
	template<typename T, typename F>
	bool dispatch(const F& func)
	{

		if (m_event.getEventType() == T::getStaticType())
		{
			m_event.handled |= func(static_cast<T&>(m_event));
			return true;
		}
		return false;
	}
private:
	Event& m_event;
};


inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.toString();
}

}