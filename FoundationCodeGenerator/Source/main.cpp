#include "CodeGenerator.h"
#include <array>
#include <filesystem>

// Just for quick debugging purposes when making local changes to the code generator
// Change to 1 to test local changes in this project quickly using Foundation\Source\ as the input folder
// Console window will also remain open when finished so you can inspect the output
#define TEST_LOCAL_CHANGES 0

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

#if TEST_LOCAL_CHANGES
	const std::string directory = "..\\Foundation\\Source\\";
	const fs::path directoryPath = fs::path(directory);
	codeGenerator.AddDirectory(directoryPath);
#else
	for (int iA = 1; iA < argc; ++iA)
	{
		const std::string directory = argv[iA];
		const fs::path directoryPath = fs::path(directory);
		codeGenerator.AddDirectory(directoryPath);
	}
#endif

	codeGenerator.AddMacro("FVARIABLE");

	int result = -1;
	if (codeGenerator.Generate())	
		result = 0;

#if TEST_LOCAL_CHANGES
	system("pause");
#endif
	return result;
}