// Created by jhelland (1/14/19)
//


#ifndef __UTIL_FUNCTIONS_H__
#define __UTIL_FUNCTIONS_H__


#include <algorithm>  // prev_permutation
#include <vector>
#include <unordered_set>
#include <assert.h>


namespace utils {

	namespace {
		static std::random_device rd;
		static std::default_random_engine gen(rd());
		static std::uniform_real_distribution<float> dist(-1.f, 1.f);
	}


	float rand() { return dist(gen); }


	template<typename T>
	std::vector<std::pair<T, T>> get_pairs(const std::vector<T>& inp) {
		int n = inp.size();
		if (n < 2)
			return std::vector<std::pair<T, T>>(0);

		std::vector<bool> v(n);
		std::fill(v.begin(), v.begin() + 2, true);

		std::vector<std::pair<T, T>> ret(n*(n - 1) / 2);

		do {
			std::vector<T> tmp;
			for (int i = 0; i < n; ++i)
				if (v[i])
					tmp.push_back(*(inp.begin() + i));

			ret.push_back(std::make_pair(tmp[0], tmp[1]));
		} while (std::prev_permutation(v.begin(), v.end()));

		return ret;
	}

}


#endif  // UTIL_FUNCTIONS_H