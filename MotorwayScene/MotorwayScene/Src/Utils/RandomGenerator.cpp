#include "RandomGenerator.h"

#include <random>
#include <ctime>

namespace
{
	std::mt19937 randEngine(static_cast<uint32_t>(time(0)));
}

namespace Random
{
	int GenerateInt(int min, int max)
	{
		std::uniform_int_distribution<int> randGenerator(min, max);
		return randGenerator(randEngine);
	}

	float GenerateFloat(float min, float max)
	{
		std::uniform_real_distribution<float> randGenerator(min, max);
		return randGenerator(randEngine);
	}
}