// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	Namspace to handle generating unique IDs for entities and components
//


#include "id_gen.h"


namespace id {
	namespace {
		static IdType id = 0;
	}

	const IdType generate_id() {
		return id++;
	}


	void reset() {
		id = 0;
	}
}