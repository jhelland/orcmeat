// Created by jhelland (1/6/19)
//
// DESCRIPTION:
//	Generic entity class with unique ID per instance.
//


#ifndef ENTITY_H
#define ENTITY_H


#include <unordered_map>
#include <typeinfo>

#include <SFML/Graphics.hpp>

#include "../utils/id_gen.h"  
#include "component.h"


namespace core {

	class Entity {
	protected:
		const id::IdType id;  // Instance-unique ID for indexing in global entity table
		std::unordered_map<const std::type_info*, Component*> componentsRegister;

	public:
		Entity() : id(id::generate_id()) {}
		~Entity() { clear_components(); }
		id::IdType get_id() const { return id; }

		// Component management
		template<typename T>
		void register_component() {  
			Component* component = new T;
			componentsRegister.insert({ &typeid(T), component });
		}

		template<typename T>
		T* get_component() const {
			if (has_component<T>())
				return dynamic_cast<T*>(componentsRegister.at(&typeid(T)));  // typeid will use the runtime type of object

			return nullptr;
		}

		template<typename T>
		inline bool has_component() const {
			return (bool)componentsRegister.count(&typeid(T));
		}

		template<typename T>
		void delete_component() {
			if (has_component<T>()) {
				delete componentsRegister.at(&typeid(T));
				componentsRegister.erase(&typeid(T));
			}
		}

		void clear_components();

		virtual sf::Vector2f get_position() const = 0;
		virtual void set_position(sf::Vector2f position) = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;
	};  // class Entity

}  // namespace core


#endif