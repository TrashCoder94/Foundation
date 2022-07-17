#include "CodeGenerator.h"
#include <fstream>
#include <functional>
#include <sstream>

#define USE_INTERMEDIATE_DIRECTORY 1
#define WRITE_TO_VCXPROJ_FILES 0

namespace Foundation
{
	namespace Generator
	{
		static const std::string MACRO_REFLECT = "FD_REFLECT";
		static const std::string MACRO_VARIABLE = "FVARIABLE";
		static const std::string MACRO_VARIABLE_FLAG_EDIT = "Edit";
		static const std::string MACRO_VARIABLE_FLAG_HIDDEN = "Hidden";
	}

	CodeGenerator::CodeGenerator() :
		m_directories(),
		m_fileExtensions()
	{}

	CodeGenerator::~CodeGenerator()
	{}
	
	void CodeGenerator::SetConfiguration(const std::string& configuration)
	{
		m_configuration = configuration;
	}

	void CodeGenerator::AddDirectory(const std::filesystem::path& directory)
	{
		m_directories.push_back(directory);
	}

	void CodeGenerator::AddExtension(const std::string& extension)
	{
		m_fileExtensions.push_back(extension);
	}

	void CodeGenerator::AddMacro(const std::string& macro)
	{
		m_macros.push_back(macro);
	}

	bool CodeGenerator::Generate()
	{
		if (m_directories.size() == 0)
			return false;

		for (size_t iD = 0; iD < m_directories.size(); ++iD)
		{
			const std::filesystem::path pathDirectory = m_directories[iD]; // Project/Source/...
			const std::filesystem::path parentDirectory = pathDirectory.parent_path(); // Project/Source/
			const std::filesystem::path projectDirectory = parentDirectory.parent_path(); // Project/

			const std::string& currentDirectoryName = projectDirectory.filename().string();

			std::cout << "=======================" << std::endl;
			std::cout << "Finding all header files in directory " << pathDirectory << std::endl << std::endl;

#if USE_INTERMEDIATE_DIRECTORY
			const std::string generatedDirectoryString = std::filesystem::current_path().string() + "\\Intermediate\\GeneratedCode\\" + m_configuration + "\\" + currentDirectoryName + "\\";
#else
			const std::string generatedDirectoryString = std::filesystem::current_path().string() + "\\..\\" + currentDirectoryName + "\\Source\\GeneratedCode\\";
#endif

			// Define reflection initialisation
			const fs::path generatedDirectory = std::filesystem::path(generatedDirectoryString);
			const std::string generatedFilename = "ReflectionData_gen.cpp";
			const std::string generatedFilepath = generatedDirectory.string() + "\\" + generatedFilename;
			const std::filesystem::path generatedParentDirectory = std::filesystem::path(generatedFilepath).parent_path();
			
			// Create the directory if we don't already have it.
			std::filesystem::create_directories(generatedParentDirectory);

			std::ofstream reflectionDataFile(generatedFilepath);
			if (reflectionDataFile.is_open())
			{
				reflectionDataFile.clear();
				
				std::string reflectionDataString("#include \"fdpch.h\"\n");
				reflectionDataString.append("#include <Foundation/Objects/Object.h>\n\n");
				const size_t stringIncludeIndex = reflectionDataString.size();
				reflectionDataString.append("namespace reflect{\n");
				reflectionDataString.append("\tvoid InitApplicationReflection()\n");
				reflectionDataString.append("\t{\n");
				reflectionDataString.append("\t\tFoundation::Object* pObject = new Foundation::Object();\n");
				
				for (auto& p : fs::recursive_directory_iterator(pathDirectory))
				{
					if (!ReadFile(currentDirectoryName, p, reflectionDataString, stringIncludeIndex))
						return false;
				}
				
				reflectionDataString.append("\t\tdelete pObject;\n");
				reflectionDataString.append("\t\tpObject = nullptr;\n");
				reflectionDataString.append("\t}\n");
				reflectionDataString.append("}\n");

				reflectionDataFile << reflectionDataString;
				reflectionDataFile.close();
			}

			std::cout << std::endl;
			std::cout << "End of header files in directory " << pathDirectory << std::endl;
			std::cout << "=======================" << std::endl;
		}

		return true;
	}

	bool CodeGenerator::ReadFile(const std::string& generationDirectoryName, const std::filesystem::directory_entry& file, std::string& reflectedStringData, const size_t reflectedStringHeaderIncludeIndex)
	{
		std::ifstream ifStreamFile(file.path());
		if (ifStreamFile.is_open())
		{
			const auto func = [this, &ifStreamFile, &file, &generationDirectoryName, &reflectedStringData, &reflectedStringHeaderIncludeIndex]()
			{
#if USE_INTERMEDIATE_DIRECTORY
				const std::string generatedDirectoryString = std::filesystem::current_path().string() + "\\Intermediate\\GeneratedCode\\" + m_configuration + "\\" + generationDirectoryName + "\\";
#else
				const std::string generatedDirectoryString = std::filesystem::current_path().string() + "\\..\\" + generationDirectoryName + "\\Source\\GeneratedCode\\";
#endif
				const std::filesystem::path generatedDirectory = std::filesystem::path(generatedDirectoryString);
				const std::string generatedFilename = file.path().stem().string() + "_gen.cpp";
				const std::string generatedFilepath = generatedDirectory.string() + generatedFilename;
				const std::filesystem::path generatedParentDirectory = std::filesystem::path(generatedFilepath).parent_path();
				const std::string className = file.path().filename().stem().string();
				std::string generatedCode = "";

				// Create the directory if we don't already have it.
				std::filesystem::create_directories(generatedParentDirectory);

				const size_t projectDirectoryOffset = 7;
				const std::string fileHeaderIncludeFullFilepath = file.path().string();
				const size_t headerFilepathStart = fileHeaderIncludeFullFilepath.find("Source") + projectDirectoryOffset;
				const size_t headerFilepathEnd = fileHeaderIncludeFullFilepath.find_last_of(".h") + 1;
				const size_t headerFilepathCount = headerFilepathEnd - headerFilepathStart;

				const std::string fileHeaderIncludeFilepath = fileHeaderIncludeFullFilepath.substr(headerFilepathStart, headerFilepathCount);

				const std::string precompiledHeaderInclude = "#include \"fdpch.h\"";
				const std::string fileHeaderInclude = "#include <" + fileHeaderIncludeFilepath + ">";
				const std::string reflectHeaderInclude = "#include <Reflect.h>";

				if (file.path().string().find("Component", 0) != std::string::npos
					&& file.path().filename() != "Component.h")
				{
					// Add this component to the object in the dummy scene.
					reflectedStringData.insert(reflectedStringHeaderIncludeIndex, fileHeaderInclude + "\n");
					reflectedStringData.append("\t\tpObject->AddComponent<Foundation::" + className + ">();\n");
				}

				if (file.path().string().find("Object", 0) != std::string::npos
					&& file.path().filename() != "BaseObject.h"
					&& file.path().filename() != "Object.h")
				{
					// Add this object in the dummy scene.
					reflectedStringData.insert(reflectedStringHeaderIncludeIndex, fileHeaderInclude + "\n");
					reflectedStringData.append("\t\tFoundation::" + className + "* p" + className + " = new Foundation::" + className + "();\n");
					reflectedStringData.append("\t\tdelete p" + className + ";\n");
					reflectedStringData.append("\t\tp" + className + " = nullptr;\n");
				}

				generatedCode.append(precompiledHeaderInclude + "\n");
				generatedCode.append(fileHeaderInclude + "\n");
				generatedCode.append(reflectHeaderInclude + "\n");
				generatedCode.append("\n");
				generatedCode.append("namespace Foundation\n");
				generatedCode.append("{\n");
				generatedCode.append("\tFD_REFLECT_BEGIN(" + className + ")\n");

				bool foundReflectionMacro = false;

				// Recursive function to populate children classes with their reflected parent properties.
				std::function<void(std::ifstream& inputFile, std::string& reflectionStringToWriteTo, bool& bFoundReflectionMacro, bool beingCalledFromChildClass, const std::string& parentClassName)> readFileFunc = [this, &readFileFunc](std::ifstream& inputFile, std::string& reflectionStringToWriteTo, bool& bFoundReflectionMacro, bool beingCalledFromChildClass = false, std::string parentClassName = "")
				{
					if (!inputFile.is_open())
					{
						return;
					}

					std::ostringstream os;
					bool foundMacro = false;
					std::string macroLine = "";
					bool foundParentClass = false;
					std::string localParentClassName = "";
					std::string parentClassHeaderFile = "";
					std::vector<std::string> headerFiles;
					const std::string& inheritingStringToLookFor = " : public ";
					std::string line = "";

					while (std::getline(inputFile, line))
					{
						if (line.find(".h\"") != std::string::npos)
							headerFiles.push_back(line);

						if (line.find(Generator::MACRO_REFLECT) != std::string::npos)
							bFoundReflectionMacro = true;

						if (line.find(inheritingStringToLookFor) != std::string::npos)
						{
							size_t inheritingStringIndex = line.find(inheritingStringToLookFor, 0);
							localParentClassName = line.substr(inheritingStringIndex + inheritingStringToLookFor.size(), line.size());
							foundParentClass = true;
						}

						// Only scan our header file if there is a reflection macro found
						if (bFoundReflectionMacro)
						{
							if (foundParentClass)
							{
								std::cout << "Parent Class Name = " << localParentClassName << std::endl;

								for (const std::string& headerFile : headerFiles)
								{
									if (headerFile.find(localParentClassName, 0) != std::string::npos)
									{
										const std::string& includeString = "#include \"";
										const size_t includeStringIndex = includeString.size();
										const size_t includeHeaderFilepathEnd = headerFile.find_last_of(".h") + 1;
										const size_t includeHeaderFilepathCount = includeHeaderFilepathEnd - includeStringIndex;

										std::string removeIncludeAndQuotes = headerFile.substr(includeStringIndex, includeHeaderFilepathCount);
										
										parentClassHeaderFile = removeIncludeAndQuotes;
										break;
									}
								}

								// If we have actually found a valid header file
								if (parentClassHeaderFile != "")
								{
									// Then we need to read whatever reflected variables the parent has too.
									for (const fs::path& dir : m_directories)
									{
										const fs::path& fullParentHeaderFilepath = fs::path(dir.string() + parentClassHeaderFile);
										if (std::filesystem::exists(fullParentHeaderFilepath))
										{
											parentClassHeaderFile = fullParentHeaderFilepath.string();
											std::cout << "Code generator managed to find parent class file successfully" << parentClassHeaderFile << std::endl;
											break;
										}
									}

									std::ifstream& parentFile = std::ifstream(parentClassHeaderFile);
									if (parentFile.is_open())
									{
										readFileFunc(parentFile, reflectionStringToWriteTo, bFoundReflectionMacro, true, localParentClassName);
									}
									else
									{
										std::cout << "Code generator failed to find parent header file: " << parentClassHeaderFile << std::endl;
									}
								}

								foundParentClass = false;
							}

							for (const std::string& macro : m_macros)
							{
								if (line.find(macro, 0) != std::string::npos)
								{
									foundMacro = true;
									macroLine = line;
									break;
								}
								else
								{
									if (foundMacro)
									{
										if (macroLine.find(Generator::MACRO_VARIABLE) != std::string::npos)
										{
											if (macroLine.find(Generator::MACRO_VARIABLE_FLAG_EDIT) != std::string::npos)
											{
												const size_t endOfVariable = line.find_last_of(";");
												const size_t startOfVariable = line.find_last_of(" ") + 1;
												const size_t variableNameLength = endOfVariable - startOfVariable;

												std::string variableName = line.substr(startOfVariable, variableNameLength);
												//std::cout << "Found variable called " << variableName << std::endl;
												
												bool isParentMember = beingCalledFromChildClass;
												if (isParentMember)
												{
													reflectionStringToWriteTo.append("\tFD_REFLECT_PARENT_MEMBER(" + parentClassName + ", " + variableName + ")\n");
												}
												else
												{
													reflectionStringToWriteTo.append("\tFD_REFLECT_MEMBER(" + variableName + ")\n");
												}
											}
										}
										foundMacro = false;
										break;
									}
								}
							}
						}
					};

					inputFile.close();
				};
				readFileFunc(ifStreamFile, generatedCode, foundReflectionMacro, false, "");

				generatedCode.append("\tFD_REFLECT_END()\n");
				generatedCode.append("}\n");

				if (foundReflectionMacro)
				{
					std::ofstream ofStreamFile(generatedFilepath);
					if (ofStreamFile.is_open())
					{
						std::cout << "Generating cpp file " << generatedFilepath << std::endl;

						ofStreamFile.clear();
						ofStreamFile << generatedCode;
						ofStreamFile.close();
					}

#if WRITE_TO_VCXPROJ_FILES
					// Include these cpp files into the project (vcxproj)
					std::cout << "Attempting to access " << vcxprojFile << std::endl;
					ifStreamFile = std::ifstream(vcxprojFile);
					std::vector<std::string> vcxprojLines;
					if (ifStreamFile.is_open())
					{
						// <ClCompile Include="Source\Foundation\Components\CameraComponent.cpp" />
						// <ClCompile Include="Source\Foundation\Components\CameraComponent.cpp" />
						std::cout << "Filepath to search for: " << sourceFilePath << std::endl;
						const std::string& clCompileLineToFind = "<ClCompile Include=\"" + sourceFilePath.string() + "\" />";
						const std::string& clCompileLineToInclude = "<ClCompile Include=\"" + generatedFilepath + "\" />";
						std::cout << "ClCompile line to search for: " << clCompileLineToFind << std::endl;
						std::cout << "ClCompile line to include: " << clCompileLineToInclude << std::endl;

						while (getline(ifStreamFile, line))
						{
							vcxprojLines.push_back(line + "\n");

							if (line.find(clCompileLineToFind) != std::string::npos)
							{
								std::cout << "Found ClCompile line " << clCompileLineToFind << " Inserting new ClCompile line: " << clCompileLineToInclude << std::endl;
								vcxprojLines.push_back(clCompileLineToInclude + "\n");
							}
						}
						ifStreamFile.close();
					}

					ofStreamFile = std::ofstream(vcxprojFile);
					if (ofStreamFile.is_open())
					{
						ofStreamFile.clear();
						for (const std::string& vcxprojLine : vcxprojLines)
						{
							ofStreamFile << vcxprojLine;
						}
						ofStreamFile.close();
					}

					std::cout << "Attempting to access " << vcxprojFiltersFile << std::endl;
					ifStreamFile = std::ifstream(vcxprojFiltersFile);
					std::vector<std::string> vcxprojFiltersLines;
					if (ifStreamFile.is_open())
					{
						const fs::path& sourceFileParentPath = sourceFilePath.parent_path();
						/*
							<ClCompile Include="Source\Foundation\Components\CameraComponent.cpp">
								<Filter>Source\Foundation\Components</Filter>
							</ClCompile>
						*/

						const std::string& clCompileLineToFind = "<ClCompile Include=\"" + sourceFilePath.string() + "\">";
						const std::string& clCompileLineToWaitFor = "</ClCompile>";
						std::string clCompileLineToInclude = "<ClCompile Include=\"" + generatedFilepath + "\">\n";
						clCompileLineToInclude.append("\t<Filter>" + generatedParentDirectory.string() + "</Filter>\n");
						clCompileLineToInclude.append("</ClCompile>\n");
						bool bFoundCLCompileLine = false;

						while (getline(ifStreamFile, line))
						{
							vcxprojFiltersLines.push_back(line + "\n");
							if (line.find(clCompileLineToFind) != std::string::npos)
							{
								std::cout << "Found ClCompile line: " << clCompileLineToFind << std::endl;
								bFoundCLCompileLine = true;
							}
							if (bFoundCLCompileLine)
							{
								if (line.find(clCompileLineToWaitFor) != std::string::npos)
								{
									std::cout << "Found ClCompile line: " << clCompileLineToWaitFor << " Inserting new ClCompile Lines:" << clCompileLineToInclude << std::endl;
									vcxprojFiltersLines.push_back(clCompileLineToInclude + "\n");
									bFoundCLCompileLine = false;
								}
							}
						}
						ifStreamFile.close();
					}

					ofStreamFile = std::ofstream(vcxprojFiltersFile);
					if (ofStreamFile.is_open())
					{
						ofStreamFile.clear();
						for (const std::string& vcxprojFilterLine : vcxprojFiltersLines)
						{
							ofStreamFile << vcxprojFilterLine;
						}
						ofStreamFile.close();
					}
#endif
				}

				ifStreamFile.close();
			};

			// If we don't care about extensions.
			if (m_fileExtensions.empty())
			{
				func();
			}
			else
			{
				for (const std::string& extension : m_fileExtensions)
				{
					if (file.path().extension() != extension)
						continue;

					func();
				}
			}
		}

		return true;
	}
};