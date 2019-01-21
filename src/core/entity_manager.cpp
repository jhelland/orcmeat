// Created by jhelland (1/21/19)
//


#include "entity_manager.h"


namespace core {

	EntityManager::EntityManager() {
		size_t maxEntitySize = 0;
		chunkSize = sizeof(utils::LargestType<PlayerEntity, CircleEntity, RectangleEntity>::type);
	}


	EntityManager::~EntityManager() {
		delete allocator;
	}


	void EntityManager::delete_entity(id::IdType id) {
		entitiesRegister[id]->~Entity();  // YOU HAVE TO DESTRUCT EXPLICITY WITH PLACEMENT NEW, YOU IDIOT
		allocator->dealloc(entitiesRegister.at(id));
		entitiesRegister.erase(id);
	}


	void EntityManager::clear_entities() {
		for (auto& it : entitiesRegister) {
			it.second->~Entity();  // placement new needs explicit destructor call
		}
		entitiesRegister.clear();

		if (allocator != nullptr) {
			allocator->reset();
		}
	}


	std::vector<Entity*> EntityManager::get_entities_by_ids(std::vector<id::IdType> ids) const {
		std::vector<Entity*> entities(ids.size());
		int i = 0;
		for (auto&& id : ids) {
			entities[i] = get_entity_by_id(id);
			++i;
		}
		return entities;
	}


	bool EntityManager::has_entity(id::IdType id) const { 
		return (bool)entitiesRegister.count(id); 
	}


	Entity* EntityManager::get_entity_by_id(id::IdType id) const { 
		return has_entity(id) ? entitiesRegister.at(id) : nullptr; 
	}


	int EntityManager::get_number_of_entities() const { 
		return entitiesRegister.size(); 
	}


	std::unordered_map<id::IdType, Entity*> EntityManager::get_entity_register() const { 
		return entitiesRegister; 
	}

}  // namespace core
