// Created by jhelland (1/8/19)
//


#include "entity.h"


namespace core {

	void Entity::clear_components() {
		for (auto& it : componentsRegister)
			delete it.second;

		componentsRegister.clear();
	}

}  // namespace core
