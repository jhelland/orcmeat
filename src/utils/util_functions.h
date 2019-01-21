// Created by jhelland (1/14/19)
//


#ifndef __UTIL_FUNCTIONS_H__
#define __UTIL_FUNCTIONS_H__


#include <random>
#include <type_traits>


namespace utils {

	float rand();

	// Get largest type from list at compile time
	template<typename... TYPES>
	struct LargestType;

	template<typename T>
	struct LargestType<T> {
		using type = T;
	};

	template<typename T, typename U, typename... TYPES>
	struct LargestType<T, U, TYPES...> {
		using type = typename LargestType<typename std::conditional<
				(sizeof(U) <= sizeof(T)), T, U
			>::type, TYPES...
		>::type;
	};

}


#endif  // UTIL_FUNCTIONS_H