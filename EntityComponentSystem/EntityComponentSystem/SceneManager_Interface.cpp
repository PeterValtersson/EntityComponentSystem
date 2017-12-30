#include <Managers\SceneManager_Interface.h>
#include "SceneManager.h"
DECLDIR_ECS ECS::SceneManager_Interface * SceneManager_CreateSceneManager_C(ECS::SceneManagerInitializationInfo ii)
{
	return new ECS::SceneManager(ii);
}

DECLDIR_ECS void SceneManager_Create_C(ECS::SceneManager_Interface* sm, uint32_t entity)
{
	sm->Create(entity);
}

DECLDIR_ECS void SceneManager_AddEntityToScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t entity)
{
	sm->AddEntityToScene(scene, entity);
}

DECLDIR_ECS void SceneManager_AddEntitiesToScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t * entities, uint32_t numEntities)
{
	sm->AddEntitiesToScene(scene, (ECS::Entity*)entities, numEntities);
}

DECLDIR_ECS uint32_t SceneManager_GetNumberOfEntitiesInScene_C(ECS::SceneManager_Interface * sm, uint32_t entity)
{
	return sm->GetNumberOfEntitiesInScene(entity);
}

DECLDIR_ECS void SceneManager_GetEntitiesInScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t * entities)
{
	sm->GetEntitiesInScene(scene, (ECS::Entity*)entities);
}

DECLDIR_ECS void SceneManager_RegisterManager_C(ECS::SceneManager_Interface * sm, ECS::Manager_Base * mb)
{
	sm->RegisterManager(mb);
}
