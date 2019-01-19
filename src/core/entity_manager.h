// Created by jhelland (1/9/19)
//
// DESCRIPTION
//		Helper class for managing entity tables and allocating/freeing memory


#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__


#include <unordered_map>
#include <typeinfo>

#include "entity.h"
#include "allocators/freelist_allocator.h"
#include "allocators/linear_allocator.h"
#include "allocators/pool_allocator.h"
#include "../utils/id_gen.h"

#include <iostream>

namespace core {

	class EntityManager {
	private:
		const unsigned int numPoolObjs;
		memory::LinearAllocator* allocator = nullptr;
		//memory::PoolAllocator* allocator = nullptr;
		//memory::FreeListAllocator* allocator = nullptr;
		std::unordered_map<id::IdType, Entity*> entitiesRegister;

	public:
		EntityManager(unsigned int numPoolObjs = 100) : numPoolObjs(numPoolObjs) {}

		~EntityManager() {
			//clear_entities();
			delete allocator;
		}

		template<typename T>
		inline void register_entity_type(const size_t numExist = 100) {
			if (allocator == nullptr) {
				size_t totalSize = numExist * sizeof(T);
				allocator = new memory::LinearAllocator(totalSize);
				//allocator = new memory::PoolAllocator(totalSize, sizeof(T));
				//auto policy = memory::FreeListAllocator::FIND_BEST;
				//allocator = new memory::FreeListAllocator(totalSize, policy);
				allocator->init();
			}
		}

		template<typename T, typename... ARGS>
		Entity* add_entity(ARGS&&... args) {
			// Make sure entity type exists in map
			register_entity_type<T>(2000);  

			// Get memory to allocate new entity
			void* newEntityMem = allocator->alloc(sizeof(T));  // Does padding matter here? YES, YES IT DOES: use defaults

			// Create entity in place and add to global register
			Entity* entity = new(newEntityMem) T(std::forward<ARGS>(args)...);
			entitiesRegister.insert({ entity->get_id(), entity });

			return entity;
		}

		void delete_entity(id::IdType id) {
			entitiesRegister[id]->~Entity();  // YOU HAVE TO DESTRUCT EXPLICITY WITH PLACEMENT NEW, YOU IDIOT
			allocator->dealloc(entitiesRegister.at(id));
			entitiesRegister.erase(id);
		}

		void clear_entities() {
			for (auto& it : entitiesRegister) {
				it.second->~Entity();  // placement new needs explicit destructor call
			}
			entitiesRegister.clear();

			if (allocator != nullptr) {
				allocator->reset();
			}
		}
		
		inline bool has_entity(id::IdType id) const { return (bool)entitiesRegister.count(id); }

		inline Entity* get_entity_by_id(id::IdType id) const { return has_entity(id) ? entitiesRegister.at(id) : nullptr; }
		
		std::vector<Entity*> get_entities_by_ids(std::vector<id::IdType> ids) const {
			std::vector<Entity*> entities(ids.size());
			int i = 0;
			for (auto&& id : ids) {
				entities[i] = get_entity_by_id(id);
				++i;
			} 
			return entities;
		}

		inline int get_number_of_entities() const { return entitiesRegister.size(); }

		inline auto get_entity_register() const { return &entitiesRegister; }
	};
	
}  // namespace core


#endif  // __ENTITY_MANAGER_H__
