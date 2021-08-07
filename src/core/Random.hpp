#pragma once
#include<random>

namespace tutorial
{

	class Random
	{
	public:
		Random() : _engine() {}
		Random(unsigned seed) : _engine(seed) {}
		virtual ~Random() {}
		template<class T>
		T get(const T &min, const T &max)
		{
			if (std::is_same<int, T>())
			{
				std::uniform_int_distribution<int> distr((int)min, (int)max);
				return T(distr(_engine));
			}
			else if (std::is_same<float, T>())
			{
				std::uniform_real_distribution<float> distr((float)min, (float)max);
				return T(distr(_engine));
			}

			std::uniform_int_distribution<unsigned> distr;
			return T(distr(_engine));
		}
	private:
		std::default_random_engine _engine;
	};

}
