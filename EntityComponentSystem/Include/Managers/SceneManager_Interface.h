#ifndef _ECS_SCENE_MANAGER_INTERFACE_H_
#define _ECS_SCENE_MANAGER_INTERFACE_H_
#include <Entity.h>
#include <DLLExport.h>
#include "Manager_Base.h"
#include "TransformManager_Interface.h"
#include <Resource.h>

namespace ECS
{
	struct SceneManager_Init_Info {
		std::shared_ptr <EntityManager_Interface> entityManager;
		std::shared_ptr <TransformManager_Interface> transformManager;
		void* pNext = nullptr;
	};
	class SceneManager_Interface : public Manager_Base {
	public:
		DECLDIR_ECS static std::shared_ptr<SceneManager_Interface> create_manager( SceneManager_Init_Info init_info );

		virtual ~SceneManager_Interface() {}

		virtual	void Create( Entity entity, const std::string& name )noexcept = 0;
		virtual void AddEntityToScene( Entity scene, Entity entity )noexcept = 0;
		virtual void AddEntitiesToScene( Entity scene, const Entity entities[], size_t numEntities )noexcept = 0;
		virtual void AddEntitiesToScene( Entity scene, const std::vector<Entity>& entities )noexcept = 0;
		virtual void AddEntityToScene( Entity scene, Entity entity, const std::string& name )noexcept = 0;
		virtual void RemoveEntityFromScene( Entity scene, Entity entity )noexcept = 0;
		virtual size_t GetNumberOfEntitiesInScene( Entity scene )const noexcept = 0;
		virtual void GetEntitiesInScene( Entity scene, Entity entities[], size_t numEntities )const noexcept = 0;
		virtual std::vector<Entity> GetEntitiesInScene( Entity scene )const noexcept = 0;
		virtual void SetNameOfScene( Entity scene, const std::string& name )noexcept = 0;
		virtual std::string GetNameOfScene( Entity scene )const noexcept = 0;
		virtual void SetNameOfEntityInScene( Entity scene, Entity entity, const std::string& name )noexcept = 0;
		virtual std::string GetNameOfEntityInScene( Entity scene, Entity entity )const noexcept = 0;
		virtual size_t GetNumberOfChildResourcesOfSceneResource( Resources::Resource resource )const = 0;
		virtual void GetChildResourcesOfSceneResource( Resources::Resource resource, Utilities::GUID resources[], uint32_t num )const = 0;
		virtual void RegisterManager( Manager_Base* manager )noexcept = 0;
	protected:
		SceneManager_Interface() {}
	};
}
//
//DECLDIR_ECS_C ECS::SceneManager_Interface* SceneManager_CreateSceneManager_C(ECS::SceneManager_Init_Info ii);
//DECLDIR_ECS_C void SceneManager_Create_C(ECS::SceneManager_Interface* sm, uint32_t entity, const char* name);
//DECLDIR_ECS_C void SceneManager_AddEntityToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity);
//DECLDIR_ECS_C void SceneManager_AddEntitiesToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t* entities, size_t numEntities);
//DECLDIR_ECS_C void SceneManager_AddNamedEntityToScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity, const char* name);
//DECLDIR_ECS_C void SceneManager_RemoveEntityFromScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity);
//DECLDIR_ECS_C uint32_t SceneManager_GetNumberOfEntitiesInScene_C(ECS::SceneManager_Interface* sm, uint32_t entity);
//DECLDIR_ECS_C void SceneManager_GetEntitiesInScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t* entities);
//DECLDIR_ECS_C void SceneManager_RegisterManager_C(ECS::SceneManager_Interface* sm, ECS::Manager_Base* mb);
//DECLDIR_ECS_C void SceneManager_SetNameOfScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, const char* name);
//DECLDIR_ECS_C void SceneManager_SetNameOfEntityScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity, const char* name);
//DECLDIR_ECS_C const char* SceneManager_GetNameOfScene_C(ECS::SceneManager_Interface* sm, uint32_t scene);
//DECLDIR_ECS_C const char* SceneManager_GetNameOfEntityInScene_C(ECS::SceneManager_Interface* sm, uint32_t scene, uint32_t entity);
//DECLDIR_ECS_C uint32_t SceneManager_GetNumberOfChildResourcesOfSceneResource_C(ECS::SceneManager_Interface* sm, uint32_t guid);
//DECLDIR_ECS_C void SceneManager_GetChildResourcesOfSceneResource_C(ECS::SceneManager_Interface* sm, uint32_t guid, uint32_t* resources, uint32_t numResources);

#endif