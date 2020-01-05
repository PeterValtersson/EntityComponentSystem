#include <Managers\SceneManager_Interface.h>
#include "SceneManager.h"/*
DECLDIR_ECS_C ECS::SceneManager_Interface * SceneManager_CreateSceneManager_C(ECS::SceneManager_Init_Info ii)
{
	return new ECS::SceneManager(ii);
}

DECLDIR_ECS_C void SceneManager_Create_C(ECS::SceneManager_Interface* sm, uint32_t entity, const char* name)
{
	sm->Create(entity, name);
}

DECLDIR_ECS_C void SceneManager_AddEntityToScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t entity)
{
	sm->AddEntityToScene(scene, entity);
}

DECLDIR_ECS_C void SceneManager_AddEntitiesToScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t * entities, uint32_t numEntities)
{
	sm->AddEntitiesToScene(scene, (ECS::Entity*)entities, numEntities);
}

DECLDIR_ECS_C void SceneManager_AddNamedEntityToScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t entity, const char * name)
{
	sm->AddEntityToScene(scene, entity, name);
}

DECLDIR_ECS_C void SceneManager_RemoveEntityFromScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t entity)
{
	sm->RemoveEntityFromScene(scene, entity);
}

DECLDIR_ECS_C uint32_t SceneManager_GetNumberOfEntitiesInScene_C(ECS::SceneManager_Interface * sm, uint32_t entity)
{
	return sm->GetNumberOfEntitiesInScene(entity);
}

DECLDIR_ECS_C void SceneManager_GetEntitiesInScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t * entities)
{
	sm->GetEntitiesInScene(scene, (ECS::Entity*)entities);
}

DECLDIR_ECS_C void SceneManager_RegisterManager_C(ECS::SceneManager_Interface * sm, ECS::Manager_Base * mb)
{
	sm->RegisterManager(mb);
}

DECLDIR_ECS_C void SceneManager_SetNameOfScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, const char * name)
{
	sm->SetNameOfScene(scene, name);
}

DECLDIR_ECS_C void SceneManager_SetNameOfEntityScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t entity, const char * name)
{
	sm->SetNameOfEntityInScene(scene, entity, name);
}

DECLDIR_ECS_C const char * SceneManager_GetNameOfScene_C(ECS::SceneManager_Interface * sm, uint32_t scene)
{
	return sm->GetNameOfScene(scene);
}

DECLDIR_ECS_C const char * SceneManager_GetNameOfEntityInScene_C(ECS::SceneManager_Interface * sm, uint32_t scene, uint32_t entity)
{
	return sm->GetNameOfEntityInScene(scene, entity);
}

DECLDIR_ECS_C uint32_t SceneManager_GetNumberOfChildResourcesOfSceneResource_C(ECS::SceneManager_Interface * sm, uint32_t guid)
{
	return sm->GetNumberOfChildResourcesOfSceneResource(ResourceHandler::Resource(guid, sm->GetManagerType()));
}

DECLDIR_ECS_C void SceneManager_GetChildResourcesOfSceneResource_C(ECS::SceneManager_Interface * sm, uint32_t guid, uint32_t * resources, uint32_t numResources)
{
	sm->GetChildResourcesOfSceneResource(ResourceHandler::Resource(guid, sm->GetManagerType()), (Utilities::GUID*)resources, numResources);
}*/

DECLDIR_ECS std::shared_ptr<ECS::SceneManager_Interface> ECS::SceneManager_Interface::create( SceneManager_Init_Info init_info )
{
	return std::make_shared<SceneManager_Interface>( init_info );
}
