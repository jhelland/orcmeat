// Created by jhelland (1/6/19)
//
// DESCRIPTION:
//	Generic entity class with unique ID per instance.
//


#ifndef ENTITY_H
#define ENTITY_H


#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../utils/id_gen.h"  // For generating unique ID for each entity instance
#include "component.h"


namespace ecs {

	class Entity {
	protected:
		const id::EntityIdType id;  // Unique ID for indexing in global entity table
		std::unordered_map<id::EntityIdType, Component*> componentsRegister;

	public:
		Entity() : id(id::generate_id()) {}
		~Entity() { clear_components(); }

		id::EntityIdType get_id() const { return id; }

		// Component management
		inline void register_component(Component* component) { componentsRegister.insert({ component->get_id(), component }); }
		inline Component* get_component_by_id(id::EntityIdType id) const { return has_component(id) ? componentsRegister.at(id) : nullptr; }
		inline bool has_component(id::EntityIdType id) const { return (bool)componentsRegister.count(id); }
		void delete_component(id::EntityIdType id) {
			if (has_component(id)) {
				delete componentsRegister.at(id);
				componentsRegister.erase(id);
			}
		}
		void clear_components() {
			for (auto& it : componentsRegister)
				delete it.second;

			componentsRegister.clear();
		}

		virtual sf::Vector2f get_position() const = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;
	};  // class Entity

}  // namespace ecs


#endif