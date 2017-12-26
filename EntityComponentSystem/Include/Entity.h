#ifndef _ECS_ENTITY_H_
#define _ECS_ENTITY_H_
#include <stdint.h>
namespace ECS
{
	struct Entity
	{
		using GENERATION_TYPE = uint8_t;
		uint32_t id;
		static const uint32_t ENTITY_INDEX_BITS = 22;
		static const uint32_t ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;
		static const uint32_t ENTITY_GENERATION_BITS = 8 * sizeof(GENERATION_TYPE);
		static const uint32_t ENTITY_GENERATION_MASK = ((1 << ENTITY_GENERATION_BITS) - 1);
		Entity(uint32_t id) : id(id)
		{

		}
		Entity(uint8_t generation, uint32_t index) : id((static_cast<uint32_t>(generation) << ENTITY_INDEX_BITS) | index)
		{

		}
		Entity() : id(-1)
		{

		}
		inline uint8_t Gen() const
		{
			return ((this->id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK);
		}
		inline uint32_t Index() const
		{
			return (this->id & ENTITY_INDEX_MASK);
		}
		inline operator uint32_t()const { return id; }
		inline bool operator==(const Entity& rhs) const
		{
			return id == rhs.id;
		}

	};
}
#endif