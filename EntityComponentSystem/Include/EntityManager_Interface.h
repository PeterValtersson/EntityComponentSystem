#ifndef _ECS_ENTITY_MANAGER_INTERFACE_H_
#define _ECS_ENTITY_MANAGER_INTERFACE_H_
#include <stdint.h>
#include "Entity.h"
#include "DLLExport.h"
#include "Memory_Base.h"
namespace ECS
{
	class Manager_Base;
	class EntityManager_Interface : public Memory_Base
	{
	public:
		virtual ~EntityManager_Interface(){}

		virtual Entity Create()noexcept = 0;
		virtual void CreateMultiple(Entity entities[], uint32_t numEntities)noexcept = 0;
		virtual bool IsAlive(Entity entity)noexcept = 0;
		virtual void Destroy(Entity entity)noexcept = 0;
		virtual void DestroyMultiple(const Entity entities[], uint32_t numEntities)noexcept = 0;
		virtual void DestroyMultipleNow(const Entity entities[], uint32_t numEntities)noexcept = 0;
		virtual void DestroyNow(Entity entity)noexcept = 0;
		virtual void DestroyAll(bool immediate = false)noexcept = 0;
		virtual uint32_t GetNumberOfAliveEntities()const noexcept = 0;
		virtual void RegisterManagerForDestroyNow(Manager_Base* manager)noexcept = 0;
		virtual void UnregisterManagerForDestroyNow(Manager_Base* manager)noexcept = 0;
	protected:
		EntityManager_Interface(){}

	};


}
DECLDIR ECS::EntityManager_Interface* EntityManager_CreateEntityManager_C();
DECLDIR bool EntityManager_IsAlive_C(ECS::EntityManager_Interface* em, uint32_t entity);
DECLDIR uint32_t EntityManager_Create_C(ECS::EntityManager_Interface* em);
DECLDIR void EntityManager_CreateMultiple_C(ECS::EntityManager_Interface* em, uint32_t* entities, uint32_t numEntities);
DECLDIR void EntityManager_Destroy_C(ECS::EntityManager_Interface* em, uint32_t entity);
DECLDIR void EntityManager_DestroyMultiple_C(ECS::EntityManager_Interface* em, const uint32_t* entities, uint32_t numEntities);
DECLDIR void EntityManager_DestroyMultipleNow_C(ECS::EntityManager_Interface* em, const uint32_t* entities, uint32_t numEntities);
DECLDIR void EntityManager_DestroyNow_C(ECS::EntityManager_Interface* em, uint32_t entity);
DECLDIR void EntityManager_DestroyAll_C(ECS::EntityManager_Interface* em, bool immediate);
DECLDIR uint32_t EntityManager_GetNumberOfAliveEntities_C(ECS::EntityManager_Interface* em);
DECLDIR void EntityManager_RegisterManagerForDestroyNow_C(ECS::EntityManager_Interface* em, ECS::Manager_Base* manager);
DECLDIR void EntityManager_UnregisterManagerForDestroyNow_C(ECS::EntityManager_Interface* em, ECS::Manager_Base* manager);
#endif