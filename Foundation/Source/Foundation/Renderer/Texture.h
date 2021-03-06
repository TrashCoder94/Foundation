#pragma once

#include <string>
#include "Foundation/Core/Base.h"

namespace Foundation
{
	class Texture
	{
		public:
			virtual ~Texture() {}

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
			virtual uint32_t GetRendererID() const = 0;

			virtual void SetData(void* data, uint32_t size) = 0;

			virtual void Bind(uint32_t slot = 0) const = 0;
			virtual void Unbind(uint32_t slot = 0) const = 0;

			virtual const std::string& GetFilepath() const = 0;

			virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
		public:
			static SharedPtr<Texture2D> Create(uint32_t width, uint32_t height);
			static SharedPtr<Texture2D> Create(const std::string& filepath);
	};
}