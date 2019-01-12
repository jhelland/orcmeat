// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	Base class for components
//


#ifndef COMPONENTS_H
#define COMPONENTS_H


#include <vector>

#include <SFML/Graphics.hpp>

#include "../utils/id_gen.h"
#include "entity.h"


namespace core {

	// Base class to inherit from
	class Component {
	public:
		Component() {}
		virtual ~Component() {}
	};  // class Component

}  // namespace core


#endif 