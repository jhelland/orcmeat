// Created by jhelland (1/8/19)
//


#include <unordered_map>
#include <vector>
#include <typeinfo>

#include "component.h"
#include "allocators/freelist_allocator.h"
#include "../utils/id_gen.h"


namespace core {

	class ComponentManager {
	private:
		const unsigned int numAllocObjs;
		memory::FreeListAllocator* allocator = nullptr;
		std::unordered_map<id::IdType, Component*> componentsRegister;
		std::unordered_map<const type_info*, std::vector<id::IdType>> entitiesWithComponents;

	public:
		ComponentManager(unsigned numAllocObjs = 100) : numAllocObjs(numAllocObjs) {}
		~ComponentManager() { delete allocator; }

		template<typename T>
		void register_component() {

		}
	};

}
