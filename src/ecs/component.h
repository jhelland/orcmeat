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


namespace ecs {

	// Base class to inherit from
	class Component {
	protected:
		bool enabled;

	public:
		Component() : enabled(true) {}
		virtual ~Component() {};

		virtual id::EntityIdType get_id() = 0;
	};


	template<typename T>
	class ComponentBase : public Component {
	protected:
		static const id::EntityIdType id;

	public:
		ComponentBase() {};
		virtual ~ComponentBase() {};

		id::EntityIdType get_id() { return id; }
	};


	// This forces a particular component type to have a static ID
	template<typename T>
	const id::EntityIdType ComponentBase<T>::id = id::generate_id();

}


#endif 