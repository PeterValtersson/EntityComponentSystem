#ifndef _ECS_MANAGER_BASE_H_
#define _ECS_MANAGER_BASE_H_
#include <Entity.h>
#include <Memory_Base.h>
#include <DLLExport.h>
#include <EntityManager_Interface.h>
namespace ECS
{
	class Manager_Base : public Memory_Base
	{
	public:
		virtual void Destroy(Entity entity)noexcept = 0;
		virtual void DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept = 0;
	
		virtual uint32_t GetNumberOfRegisteredEntities()const noexcept = 0;
		virtual void GetRegisteredEntities(Entity entities[], uint32_t numEntities)const noexcept = 0;
	
		virtual void Frame()noexcept = 0;
	protected:
		Manager_Base(){}

		virtual void GarbageCollection()noexcept = 0;
	};
}

DECLDIR void Manager_Base_Destroy_C(ECS::Manager_Base* mb, uint32_t entity);
DECLDIR void Manager_Base_DestroyEntities_C(ECS::Manager_Base* mb, const uint32_t entities[], uint32_t numEntities);
DECLDIR uint32_t Manager_Base_GetNumberOfRegisteredEntities(ECS::Manager_Base* mb);
DECLDIR void Manager_Base_GetRegisteredEntities(ECS::Manager_Base* mb, uint32_t* entities, uint32_t numEntities);
DECLDIR void Manager_Base_Frame(ECS::Manager_Base* mb);
#endif