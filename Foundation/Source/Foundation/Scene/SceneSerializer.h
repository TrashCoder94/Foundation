#pragma once

#include "Reflect.h"

namespace Foundation
{
	class Scene;

	class SceneSerializer
	{
		public:
			SceneSerializer(const SharedPtr<Scene>& pScene);

			// Serializes to a text file.
			void Serialize(const std::string& filepath);

			// Serializes to a binary asset.
			void SerializeBinary(const std::string& filepath);

			// Serializes to a text file.
			void Deserialize(const std::string& filepath);

			// Serializes to a binary asset.
			void DeserializeBinary(const std::string& filepath);

		private:
			SharedPtr<Scene> m_pLevel;
	};
}