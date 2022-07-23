#pragma once

#include "Event.h"

namespace Foundation
{
	class SceneLoadedEvent : public Event
	{
	public:
		SceneLoadedEvent(const std::string& name) : Event(),
			m_Name(name)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SceneLoadedEvent: " << m_Name;
			return ss.str();
		}

		EVENT_CLASS_TYPE(SceneLoaded)
		EVENT_CLASS_CATEGORY(EventCategoryScene)

	private:
		std::string m_Name;
	};
}