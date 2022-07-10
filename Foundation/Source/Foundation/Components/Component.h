#pragma once

#include "Foundation/Objects/BaseObject.h"

namespace Foundation
{
	class Object;

	class Component : public BaseObject
	{
		FD_REFLECT()

		public:
			Component() : BaseObject()
			{}

			virtual ~Component()
			{}
	};
}