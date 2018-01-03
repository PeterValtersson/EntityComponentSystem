#include <EntityManager_Interface.h>
#include "EntityManager.h"
ECS::EntityManager_Interface * EntityManager_CreateEntityManager_C()
{
	return new ECS::EntityManager();
}

DECLDIR_ECS bool EntityManager_IsAlive_C(ECS::EntityManager_Interface * em, uint32_t entity)
{
	return em->IsAlive(entity);
}

uint32_t EntityManager_Create_C(ECS::EntityManager_Interface* em)
{
	return em->Create();
}

DECLDIR_ECS void EntityManager_CreateMultiple_C(ECS::EntityManager_Interface * em, uint32_t * entities, uint32_t numEntities)
{
	 em->CreateMultiple((ECS::Entity*)entities, numEntities);
}

DECLDIR_ECS void EntityManager_Destroy_C(ECS::EntityManager_Interface* em, uint32_t entity)
{
	em->Destroy(entity);
}

DECLDIR_ECS void EntityManager_DestroyMultiple_C(ECS::EntityManager_Interface * em, const uint32_t * entities, uint32_t numEntities)
{
	em->DestroyMultiple((ECS::Entity*)entities, numEntities);
}

DECLDIR_ECS void EntityManager_DestroyMultipleNow_C(ECS::EntityManager_Interface * em, const uint32_t * entities, uint32_t numEntities)
{
	em->DestroyMultipleNow((ECS::Entity*)entities, numEntities);
}

DECLDIR_ECS void EntityManager_DestroyNow_C(ECS::EntityManager_Interface* em, uint32_t entity)
{
	em->DestroyNow(entity);
}

DECLDIR_ECS void EntityManager_DestroyAll_C(ECS::EntityManager_Interface * em, bool immediate)
{
	em->DestroyAll(immediate);
}

DECLDIR_ECS uint32_t EntityManager_GetNumberOfAliveEntities_C(ECS::EntityManager_Interface * em)
{
	return  em->GetNumberOfAliveEntities();
}

DECLDIR_ECS void EntityManager_RegisterManagerForDestroyNow_C(ECS::EntityManager_Interface * em, ECS::Manager_Base * manager)
{
	em->RegisterManagerForDestroyNow(manager);
}

DECLDIR_ECS void EntityManager_UnregisterManagerForDestroyNow_C(ECS::EntityManager_Interface * em, ECS::Manager_Base * manager)
{
	em->UnregisterManagerForDestroyNow(manager);
}
