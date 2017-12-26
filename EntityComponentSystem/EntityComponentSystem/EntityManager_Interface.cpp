#include <EntityManager_Interface.h>
#include "EntityManager.h"
ECS::EntityManager_Interface * ECS::EntityManager_CreateEntityManager_C()
{
	return new EntityManager();
}

DECLDIR bool ECS::EntityManager_IsAlive_C(ECS::EntityManager_Interface * em, uint32_t entity)
{
	return em->IsAlive(entity);
}

uint32_t ECS::EntityManager_Create_C(ECS::EntityManager_Interface* em)
{
	return em->Create();
}

DECLDIR void ECS::EntityManager_Destroy_C(ECS::EntityManager_Interface* em, uint32_t entity)
{
	em->Destroy(entity);
}

DECLDIR void ECS::EntityManager_DestroyNow_C(ECS::EntityManager_Interface* em, uint32_t entity)
{
	em->DestroyNow(entity);
}

DECLDIR void ECS::EntityManager_DestroyAll_C(ECS::EntityManager_Interface * em, bool immediate)
{
	em->DestroyAll();
}

DECLDIR uint32_t ECS::EntityManager_GetNumberOfAliveEntities_C(ECS::EntityManager_Interface * em)
{
	return  em->GetNumberOfAliveEntities();
}

DECLDIR void ECS::EntityManager_RegisterManagerForDestroyNow_C(ECS::EntityManager_Interface * em, ECS::Manager_Base * manager)
{
	em->RegisterManagerForDestroyNow(manager);
}

DECLDIR void ECS::EntityManager_UnregisterManagerForDestroyNow_C(ECS::EntityManager_Interface * em, ECS::Manager_Base * manager)
{
	em->UnregisterManagerForDestroyNow(manager);
}
