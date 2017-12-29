#include "Manager_Base.h"
#include <ResourceHandler\ResourceHandler_Interface.h>
DECLDIR_ECS bool Manager_Base_IsRegistered_C(ECS::Manager_Base * mb, uint32_t entity)
{
	return mb->IsRegistered(entity);
}
DECLDIR_ECS void Manager_Base_Destroy_C(ECS::Manager_Base * mb, uint32_t entity)
{
	mb->Destroy(entity);
}

DECLDIR_ECS void Manager_Base_DestroyEntities_C(ECS::Manager_Base * mb, const uint32_t entities[], uint32_t numEntities)
{
	mb->DestroyEntities((const ECS::Entity*)entities, numEntities);
}

DECLDIR_ECS uint32_t Manager_Base_GetNumberOfRegisteredEntities(ECS::Manager_Base * mb)
{
	return mb->GetNumberOfRegisteredEntities();
}

DECLDIR_ECS void Manager_Base_GetRegisteredEntities(ECS::Manager_Base * mb, uint32_t * entities, uint32_t numEntities)
{
	mb->GetRegisteredEntities((ECS::Entity*)entities, numEntities);
}

DECLDIR_ECS void Manager_Base_Frame(ECS::Manager_Base * mb)
{
	mb->Frame();
}

DECLDIR_ECS uint32_t Manager_Base_CreateFromResource_C(ECS::Manager_Base * mb, const char * guid, const char * type)
{
	return mb->CreateFromResource(ResourceHandler::Resource(std::string(guid), std::string(type)));
}

DECLDIR_ECS void Manager_Base_WriteComponent_C(ECS::Manager_Base * mb, ResourceHandler::Loader_Interface * li, uint32_t entity, const char * guid, const char * type)
{	
	li->CreateFromCallback(guid, type, mb->GetDataWriter(entity));
}
