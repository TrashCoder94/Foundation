#include "fdpch.h"
#include "Random.h"

namespace Foundation
{
	std::mt19937 Random::s_RandomEngine;
	std::uniform_real_distribution<float> Random::s_Distribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
}