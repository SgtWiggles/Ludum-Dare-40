#pragma once
#include <random>
class RandomEngine
{
public:
	RandomEngine();
	
	template<class T>
	T get_int(T low, T high) 
	{
		std::uniform_int_distribution<T> dis(low, high);
		return dis(rand_engine);
	}

	template<class T>
	T get_int(T high) 
	{
		return get_int<T>(T(0), high);
	}

	template<class T>
	T get_real(T low, T high)
	{
		std::uniform_real_distribution<T> dis(low, high);
		return dis(rand_engine);
	}

private:

	std::random_device rd;
	std::mt19937 rand_engine;

};
