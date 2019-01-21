// Created by jhelland (1/9/19)
//
// DESCRIPTION
//		Helper class for managing entity tables and allocating/freeing memory


#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__


#include <unordered_map>
#include <typeinfo>

#ifdef _DEBUG
#include <iostream>
#endif

#include "../entities/entity_circle.h"
#include "../entities/entity_player.h"
#include "../entities/entity_rectangle.h"

#include "allocators/freelist_allocator.h"
#include "allocators/linear_allocator.h"
#include "allocators/pool_allocator.h"

#include "../utils/id_gen.h"
#include "../utils/util_functions.h"


namespace core {

	class EntityManager {
	private:
		size_t chunkSize;
		//memory::LinearAllocator* allocator = nullptr;
		memory::PoolAllocator* allocator = nullptr;
		//memory::FreeListAllocator* allocator = nullptr;
		std::unordered_map<id::IdType, Entity*> entitiesRegister;

	public:
		EntityManager();
		~EntityManager();

		void delete_entity(id::IdType id);
		void clear_entities();
		bool has_entity(id::IdType id) const;
		Entity* get_entity_by_id(id::IdType id) const;
		std::vector<Entity*> get_entities_by_ids(std::vector<id::IdType> ids) const;
		int get_number_of_entities() const;
		std::unordered_map<id::IdType, Entity*> get_entity_register() const;

		template<typename T>
		void register_entity_type(const size_t numExist = 100) {
			if (allocator == nullptr) {
				size_t totalSize = numExist * chunkSize;
				//allocator = new memory::LinearAllocator(totalSize);
				allocator = new memory::PoolAllocator(totalSize, chunkSize);
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
	};
	
}  // namespace core


#endif  // __ENTITY_MANAGER_H__
