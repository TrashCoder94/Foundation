#pragma once	

#include "Foundation/Core/PlatformDetection.h"

// Common
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

// Data Structures
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Foundation/Core/Base.h"
#include "Foundation/Core/Log.h"
#include "Foundation/Profiling/Instrumentor.h"

// Platform Specific
#ifdef FD_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
