#pragma once

#include <random>

namespace Foundation
{
	class Random
	{
		public:
			static void Init()
			{
				s_RandomEngine.seed(std::random_device()());
			}

			static float Float()
			{
				return 1.0f;
				//return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<float>::max();
			}

		private:
			static std::mt19937 s_RandomEngine;
			static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
}