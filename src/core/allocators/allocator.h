// Created by jhelland (1/10/19)
//


#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__


#include <cstddef>  // size_t


namespace core {
	namespace memory {

		class Allocator {
		protected:
			size_t totalSize;
			size_t used;
			size_t peak;

		public:
			Allocator(const size_t totalSize) : totalSize(totalSize), used(0) {}
			virtual ~Allocator() { totalSize = 0; }

			virtual void* alloc(const size_t size, const size_t alignment = 0) = 0;
			virtual void dealloc(void* ptr) = 0;
			virtual void init() = 0;
		};  // class Allocator


		namespace utils {

			static const size_t calc_padding(const size_t baseAddr, const size_t alignment) {
				const size_t multiplier = (baseAddr / alignment) + 1;
				const size_t alignedAddr = multiplier * alignment;
				const size_t padding = alignedAddr - baseAddr;
				return padding;
			}  

			
			static const size_t calc_padding_header(const size_t baseAddr, const size_t alignment, const size_t headerSize) {
				size_t padding = calc_padding(baseAddr, alignment);
				size_t neededSpace = headerSize;

				if (padding < neededSpace)
					neededSpace -= padding;  // Header doesn't fit; calculate next aligned address that header fits

				if (neededSpace % alignment > 0)
					padding += alignment * (1 + (neededSpace / alignment));
				else
					padding += alignment * (neededSpace / alignment);

				return padding;
			}  

		}  // namespace utils
	}  // namespace memory
}  // namespace core


#endif