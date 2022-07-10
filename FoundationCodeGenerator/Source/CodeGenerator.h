#pragma once

#include <Reflect.h>
#include <array>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace Foundation
{
	class CodeGenerator
	{
		public:
			CodeGenerator();
			~CodeGenerator();

			void SetConfiguration(const std::string& configuration);
			void AddDirectory(const std::filesystem::path& directory);
			void AddExtension(const std::string& extension);
			void AddMacro(const std::string& macro);

			bool Generate();

		private:
			bool ReadFile(const std::string& intermediateDirectoryName, const std::filesystem::directory_entry& file, std::string& reflectedStringData, const size_t reflectedStringHeaderIncludeIndex);

			std::vector<std::filesystem::path> m_directories;
			std::vector<std::string> m_fileExtensions;
			std::vector<std::string> m_macros;
			std::string m_configuration;
			std::string m_projectName;
	};
}