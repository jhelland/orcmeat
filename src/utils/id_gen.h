// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	Namspace to handle generating unique IDs for entities and components
//


#ifndef ID_GEN_H
#define ID_GEN_H


namespace id {
	typedef unsigned int IdType;  // Just to make code more readable

	const IdType generate_id();
	void reset();
}


#endif