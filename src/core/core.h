// Created by jhelland (1/9/19)
//
// DESCRIPTION:
//	Just a catch-all header for the core namespace
//


#ifndef __CORE_H__
#define __CORE_H__


// ECS
#include "entity.h"
#include "entity_manager.h"
#include "component.h"

// Memory management
#include "allocators/pool_allocator.h"
#include "allocators/linear_allocator.h"
#include "allocators/stack_allocator.h"
#include "allocators/freelist_allocator.h"
#include "memory_manager.h"


namespace core {}


#endif  // __CORE_H__
