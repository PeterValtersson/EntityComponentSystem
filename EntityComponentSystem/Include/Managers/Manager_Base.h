#ifndef _ECS_MANAGER_BASE_H_
#define _ECS_MANAGER_BASE_H_
#include <Entity.h>
#include <Memory_Base.h>
#include <DLLExport.h>
#include <EntityManager_Interface.h>
#include <ResourceHandler\Resource.h>
#include <functional>
#include <fstream>
namespace ECS
{
	class Manager_Base : public Memory_Base
	{
	public:
		
		virtual Entity CreateFromResource(ResourceHandler::Resource resource) = 0;
		virtual std::function<void(std::fstream& file)> GetDataWriter(Entity entity) = 0;
		virtual void Destroy(Entity entity)noexcept = 0;
		virtual void DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept = 0;
	
		virtual uint32_t GetNumberOfRegisteredEntities()const noexcept = 0;
		virtual void GetRegisteredEntities(Entity entities[], uint32_t numEntities)const noexcept = 0;
	
		virtual void Frame()noexcept = 0;
	protected:
		Manager_Base(){}

		virtual void GarbageCollection()noexcept = 0;
	};
}

DECLDIR_ECS void Manager_Base_Destroy_C(ECS::Manager_Base* mb, uint32_t entity);
DECLDIR_ECS void Manager_Base_DestroyEntities_C(ECS::Manager_Base* mb, const uint32_t entities[], uint32_t numEntities);
DECLDIR_ECS uint32_t Manager_Base_GetNumberOfRegisteredEntities(ECS::Manager_Base* mb);
DECLDIR_ECS void Manager_Base_GetRegisteredEntities(ECS::Manager_Base* mb, uint32_t* entities, uint32_t numEntities);
DECLDIR_ECS void Manager_Base_Frame(ECS::Manager_Base* mb);
DECLDIR_ECS uint32_t Manager_Base_CreateFromResource_C(ECS::Manager_Base* mb, ResourceHandler::ResourceHandler_Interface* rh, const char* guid, const char* type);
DECLDIR_ECS void Manager_Base_WriteComponentData_C(ECS::Manager_Base* mb, ResourceHandler::Loader_Interface* li, uint32_t entity, const char* guid, const char* type);

#endif