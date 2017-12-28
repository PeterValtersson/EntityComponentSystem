#include "Manager_Base.h"

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

DECLDIR_ECS void Manager_Base_CreateFromResource(ECS::Manager_Base * mb, ResourceHandler::ResourceHandler_Interface * rh, const char * guid, const char * type)
{
	
}
