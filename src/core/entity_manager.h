// Created by jhelland (1/9/19)
//
// DESCRIPTION
//		Helper class for managing entity tables and allocating/freeing memory


#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__


#include <unordered_map>
#include <typeinfo>

#include "entity.h"
//#include "core.h"
#include "allocators/freelist_allocator.h"
#include "allocators/pool_allocator.h"
#include "../utils/id_gen.h"

#include <iostream>

namespace core {

	class EntityManager {
	private:
		const unsigned int numPoolObjs;
		//memory::PoolAllocator* allocator = nullptr;
		memory::FreeListAllocator* allocator = nullptr;
		std::unordered_map<id::IdType, Entity*> entitiesRegister;

	public:
		EntityManager(unsigned int numPoolObjs = 100) : numPoolObjs(numPoolObjs) {};

		~EntityManager() {
			//clear_entities();
			delete allocator;
		}

		template<typename T>
		inline bool has_entity_type() const { return (bool)allocMap.count(&typeid(T)); }

		template<typename T>
		inline void register_entity_type() {
			if (allocator == nullptr) {
				size_t totalSize = 100 * sizeof(T);
				//allocator = new memory::PoolAllocator(totalSize, sizeof(T));
				auto policy = memory::FreeListAllocator::FIND_BEST;
				allocator = new memory::FreeListAllocator(totalSize, policy);
				allocator->init();
			}
		}

		template<typename T, typename... ARGS>
		Entity* add_entity(ARGS&&... args) {
			// Make sure entity type exists in map
			register_entity_type<T>();  

			// Get memory to allocate new entity
			void* ptrMem = allocator->alloc(sizeof(T));  // Does padding matter here? YES, YES IT DOES: use defaults

			// Create entity in place and add to global register
			Entity* entity = new(ptrMem) T(std::forward<ARGS>(args)...);
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

			if (allocator != nullptr)
				allocator->reset();
		}
		
		inline bool has_entity(id::IdType id) const { return (bool)entitiesRegister.count(id); }
		inline Entity* get_entity_by_id(id::IdType id) const { return has_entity(id) ? entitiesRegister.at(id) : nullptr; }
		inline int get_number_of_entities() const { return entitiesRegister.size(); }

		inline auto get_entity_register() { return &entitiesRegister; }
	};
	
}  // namespace core


#endif  // __ENTITY_MANAGER_H__
