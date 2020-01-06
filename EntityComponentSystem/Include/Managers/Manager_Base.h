#ifndef _ECS_MANAGER_BASE_H_
#define _ECS_MANAGER_BASE_H_
#include <Entity.h>
#include <Memory_Base.h>
#include <DLLExport.h>
#include <EntityManager_Interface.h>
#include <Resource.h>
#include <functional>
#include <fstream>
#include <Utilities/GUID.h>
#include <Utilities/ErrorHandling.h>

namespace ECS
{
	class CouldNotCreateManager : public Utilities::Exception {
	public:
		CouldNotCreateManager( std::string_view reason ) : Exception( reason ) {};
	};
	class Manager_Base : public Memory_Base
	{
	public:
		virtual bool is_registered(Entity entity)const noexcept = 0;
		virtual void CreateFromResource(Entity entity, Resources::Resource resource) = 0;
		virtual uint64_t GetDataWriter(Entity entity, std::function<bool(std::ostream& stream)>& writer)const noexcept = 0;
		virtual void Destroy(Entity entity)noexcept = 0;
		virtual void DestroyMultiple(const Entity entities[], size_t numEntities )noexcept = 0;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept = 0;
		virtual void DestroyAll()noexcept = 0;

		virtual void ToggleActive(Entity entity, bool active)noexcept = 0;
		virtual void ToggleActive(const Entity entities[], size_t numEntities, bool active)noexcept = 0;
		virtual void ToggleActive( const std::vector<Entity>& entities, bool active)noexcept = 0;

		virtual size_t GetNumberOfRegisteredEntities()const noexcept = 0;
		virtual void GetRegisteredEntities(Entity entities[], size_t numEntities)const noexcept = 0;
		virtual std::vector<Entity> GetRegisteredEntities()const noexcept = 0;
	
		virtual void Frame()noexcept = 0;

		virtual Utilities::GUID GetManagerType()const noexcept = 0;
	protected:
		Manager_Base(){}

		virtual void GarbageCollection()noexcept = 0;
	};
}
//DECLDIR_ECS_C bool Manager_Base_IsRegistered_C(ECS::Manager_Base* mb, uint32_t entity);
//DECLDIR_ECS_C void Manager_Base_Destroy_C(ECS::Manager_Base* mb, uint32_t entity);
//DECLDIR_ECS_C void Manager_Base_DestroyEntities_C(ECS::Manager_Base* mb, const uint32_t entities[], size_t numEntities);
//DECLDIR_ECS_C void Manager_Base_DestroyAll_C(ECS::Manager_Base* mb);
//DECLDIR_ECS_C void Manager_Base_ToggleActive_C(ECS::Manager_Base* mb, uint32_t entity, bool active);
//DECLDIR_ECS_C void Manager_Base_ToggleActiveMultiple_C(ECS::Manager_Base* mb, const uint32_t entities[], size_t numEntities, bool active);
//DECLDIR_ECS_C uint32_t Manager_Base_GetNumberOfRegisteredEntities_C(ECS::Manager_Base* mb);
//DECLDIR_ECS_C void Manager_Base_GetRegisteredEntities_C(ECS::Manager_Base* mb, uint32_t* entities, size_t numEntities);
//DECLDIR_ECS_C void Manager_Base_Frame_C(ECS::Manager_Base* mb);
//DECLDIR_ECS_C void Manager_Base_CreateFromResource_C(ECS::Manager_Base* mb, uint32_t entity, const char* guid, const char* type);
//DECLDIR_ECS_C void Manager_Base_CreateFromResourceG_C(ECS::Manager_Base* mb, uint32_t entity, uint32_t guid, uint32_t type);
//DECLDIR_ECS_C Utilities::Error Manager_Base_WriteComponent_C(ECS::Manager_Base* mb, ResourceHandler::FileSystem_Interface* li, uint32_t entity, const char* guid, const char* type);
//DECLDIR_ECS_C uint32_t Manager_Base_GetManagerType_C(ECS::Manager_Base* mb);
#endif