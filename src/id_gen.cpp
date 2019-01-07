// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	Namspace to handle generating unique IDs for entities and components
//


#include "id_gen.h"


namespace id {
	namespace {
		static unsigned int id = 0;
	}

	unsigned int generate_id() {
		return id++;
	}
}