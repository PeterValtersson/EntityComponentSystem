#include <Managers\SceneManager_Interface.h>

DECLDIR ECS::SceneManager_Interface * SceneManager_CreateSceneManager_C(ECS::SceneManagerInitializationInfo ii)
{
	return nullptr;
}

DECLDIR void SceneManager_Create_C(ECS::SceneManager_Interface* sm, uint32_t entity)
{
	sm->Create(entity);
}

DECLDIR void SceneManager_AddEntityToScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t entity)
{
	sm->AddEntityToScene(scene, entity);
}

DECLDIR void SceneManager_AddEntitiesToScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t * entities, uint32_t numEntities)
{
	sm->AddEntitiesToScene(scene, (ECS::Entity*)entities, numEntities);
}

DECLDIR uint32_t SceneManager_GetNumberOfEntitiesInScene_C(ECS::SceneManager_Interface * sm, uint32_t entity)
{
	return sm->GetNumberOfEntitiesInScene(entity);
}

DECLDIR void SceneManager_GetEntitiesInScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t * entities)
{
	sm->GetEntitiesInScene(scene, (ECS::Entity*)entities);
}
