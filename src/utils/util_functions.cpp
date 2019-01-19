// Created by jhelland (1/19/19)
//


#include "util_functions.h"


namespace utils {

	namespace {
		static std::random_device rd;
		static std::default_random_engine gen(rd());
		static std::uniform_real_distribution<float> dist(-1.f, 1.f);
	}

	float rand() {
		return dist(gen);
	}

}  // namespace utils
