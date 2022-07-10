#include "CodeGenerator.h"
#include <array>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
	const std::string exePath = argv[0];
	const fs::path exeFolder = fs::path(exePath).parent_path();
	const fs::path configurationFolder = exeFolder.parent_path();
	const fs::path configuration = configurationFolder.filename();

	Foundation::CodeGenerator codeGenerator;
	codeGenerator.SetConfiguration(configuration.string());
	codeGenerator.AddExtension(".h");
	codeGenerator.AddExtension(".hpp");

	for (int iA = 1; iA < argc; ++iA)
	{
		const std::string directory = argv[iA];
		const fs::path directoryPath = fs::path(directory);
		codeGenerator.AddDirectory(directoryPath);
	}

	codeGenerator.AddMacro("FVARIABLE");

	int result = -1;
	if (codeGenerator.Generate())	
		result = 0;

	return result;
}