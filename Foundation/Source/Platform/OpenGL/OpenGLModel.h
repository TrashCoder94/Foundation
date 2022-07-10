#pragma once

#include "Foundation/Renderer/Model.h"

namespace Foundation
{
	class OpenGLModel : public Model
	{
		public:
			OpenGLModel(const std::string& filepath);
			~OpenGLModel();

		protected:
			virtual unsigned int TextureFromFile(const fs::path& filepath) override;
	};
}