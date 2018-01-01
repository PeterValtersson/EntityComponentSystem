#ifndef _ECS_SCENE_MANAGER_INTERFACE_H_
#define _ECS_SCENE_MANAGER_INTERFACE_H_
#include <Entity.h>
#include <DLLExport.h>
#include "Manager_Base.h"
#include "TransformManager_Interface.h"
namespace ECS
{
	struct SceneManagerInitializationInfo
	{
		EntityManager_Interface* entityManager = nullptr;
		TransformManager_Interface* transformManager = nullptr;
		void* pNext = nullptr;
	};
	class SceneManager_Interface : public Manager_Base
	{
	public:
		virtual ~SceneManager_Interface(){}

		virtual	void Create(Entity entity, const std::string& name)noexcept = 0;
		virtual void AddEntityToScene(Entity scene, Entity entity)noexcept = 0;
		virtual void AddEntitiesToScene(Entity scene,const Entity entities[], uint32_t numEntities)noexcept = 0;
		virtual void AddEntityToScene(Entity scene, Entity entity, const std::string& name)noexcept = 0;
		virtual void RemoveEntityFromScene(Entity scene, Entity entity)noexcept = 0;
		virtual uint32_t GetNumberOfEntitiesInScene(Entity scene)const noexcept = 0;
		virtual void GetEntitiesInScene(Entity scene, Entity entities[])const noexcept = 0;
		virtual const char* GetNameOfScene(Entity scene)const noexcept = 0;
		virtual const char* GetNameOfEntityInScene(Entity scene, Entity entity)const noexcept = 0;

		virtual void RegisterManager(Manager_Base* manager)noexcept = 0;
	protected:
		SceneManager_Interface() {}
	};
}

DECLDIR_ECS ECS::SceneManager_Interface* SceneManager_CreateSceneManager_C(ECS::SceneManagerInitializationInfo ii);
DECLDIR_ECS void SceneManager_Create_C(ECS::SceneManager_Interface* sm, uint32_t entity, const char* name);
DECLDIR_ECS void SceneManager_AddEntityToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity);
DECLDIR_ECS void SceneManager_AddEntitiesToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t* entities, uint32_t numEntities);
DECLDIR_ECS void SceneManager_AddNamedEntityToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity, const char* name);
DECLDIR_ECS void SceneManager_RemoveEntityFromScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity);
DECLDIR_ECS uint32_t SceneManager_GetNumberOfEntitiesInScene_C(ECS::SceneManager_Interface* sm, uint32_t entity);
DECLDIR_ECS void SceneManager_GetEntitiesInScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t* entities);
DECLDIR_ECS void SceneManager_RegisterManager_C(ECS::SceneManager_Interface* sm, ECS::Manager_Base* mb);
DECLDIR_ECS const char* SceneManager_GetNameOfScene_C(ECS::SceneManager_Interface* sm, uint32_t scene);
DECLDIR_ECS const char* SceneManager_GetNameOfEntityInScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity);
#endif