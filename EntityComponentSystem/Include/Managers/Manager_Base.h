#ifndef _ECS_MANAGER_BASE_H_
#define _ECS_MANAGER_BASE_H_
#include <Entity.h>

namespace ECS
{
	class Manager_Base
	{
	public:
		virtual void Destroy(Entity entity) = 0;
		virtual void DestroyEntities(const Entity entities[], uint32_t numEntities) = 0;
	protected:
		Manager_Base(){}
	};
}
#endif