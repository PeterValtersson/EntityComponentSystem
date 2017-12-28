#ifndef _ECS_SCENE_MANAGER_INTERFACE_H_
#define _ECS_SCENE_MANAGER_INTERFACE_H_
#include <Entity.h>
#include <DLLExport.h>
#include "Manager_Base.h"
namespace ECS
{
	struct SceneManagerInitializationInfo
	{
		EntityManager_Interface* entityManager;
		void* pNext;
	};
	class SceneManager_Interface : public Manager_Base
	{
	public:
		virtual ~SceneManager_Interface(){}

		virtual	void Create(Entity entity)noexcept = 0;
		virtual void AddEntityToScene(Entity scene, Entity entity)noexcept = 0;
		virtual void AddEntitiesToScene(Entity scene,const Entity entities[], uint32_t numEntities)noexcept = 0;
		//virtual void AddNamedEntityToScene(Entity scene, Entity entity, Utilz::GUID guid)noexcept = 0;
		virtual uint32_t GetNumberOfEntitiesInScene(Entity scene)const noexcept = 0;
		virtual void GetEntitiesInScene(Entity scene, Entity entities[])const noexcept = 0;
		
	protected:
		SceneManager_Interface() {}
	};
}

DECLDIR_ECS ECS::SceneManager_Interface* SceneManager_CreateSceneManager_C(ECS::SceneManagerInitializationInfo ii);
DECLDIR_ECS void SceneManager_Create_C(ECS::SceneManager_Interface* sm, uint32_t entity);
DECLDIR_ECS void SceneManager_AddEntityToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity);
DECLDIR_ECS void SceneManager_AddEntitiesToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t* entities, uint32_t numEntities);
DECLDIR_ECS uint32_t SceneManager_GetNumberOfEntitiesInScene_C(ECS::SceneManager_Interface* sm, uint32_t entity);
DECLDIR_ECS void SceneManager_GetEntitiesInScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t* entities);
#endif