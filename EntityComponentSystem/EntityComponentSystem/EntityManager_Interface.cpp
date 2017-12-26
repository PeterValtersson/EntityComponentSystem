#include <EntityManager_Interface.h>
#include "EntityManager.h"
ECS::EntityManager_Interface * EntityManager_CreateEntityManager_C()
{
	return new ECS::EntityManager();
}

DECLDIR bool EntityManager_IsAlive_C(ECS::EntityManager_Interface * em, uint32_t entity)
{
	return em->IsAlive(entity);
}

uint32_t EntityManager_Create_C(ECS::EntityManager_Interface* em)
{
	return em->Create();
}

DECLDIR void EntityManager_Destroy_C(ECS::EntityManager_Interface* em, uint32_t entity)
{
	em->Destroy(entity);
}

DECLDIR void EntityManager_DestroyNow_C(ECS::EntityManager_Interface* em, uint32_t entity)
{
	em->DestroyNow(entity);
}

DECLDIR void EntityManager_DestroyAll_C(ECS::EntityManager_Interface * em, bool immediate)
{
	em->DestroyAll();
}

DECLDIR uint32_t EntityManager_GetNumberOfAliveEntities_C(ECS::EntityManager_Interface * em)
{
	return  em->GetNumberOfAliveEntities();
}

DECLDIR void EntityManager_RegisterManagerForDestroyNow_C(ECS::EntityManager_Interface * em, ECS::Manager_Base * manager)
{
	em->RegisterManagerForDestroyNow(manager);
}

DECLDIR void EntityManager_UnregisterManagerForDestroyNow_C(ECS::EntityManager_Interface * em, ECS::Manager_Base * manager)
{
	em->UnregisterManagerForDestroyNow(manager);
}
