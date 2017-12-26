#include "Manager_Base.h"

DECLDIR void Manager_Base_Destroy_C(ECS::Manager_Base * mb, uint32_t entity)
{
	mb->Destroy(entity);
}

DECLDIR void Manager_Base_DestroyEntities_C(ECS::Manager_Base * mb, const uint32_t entities[], uint32_t numEntities)
{
	mb->DestroyEntities((const ECS::Entity*)entities, numEntities);
}

DECLDIR uint32_t Manager_Base_GetNumberOfRegisteredEntities(ECS::Manager_Base * mb)
{
	return mb->GetNumberOfRegisteredEntities();
}

DECLDIR void Manager_Base_GetRegisteredEntities(ECS::Manager_Base * mb, uint32_t * entities, uint32_t numEntities)
{
	mb->GetRegisteredEntities((ECS::Entity*)entities, numEntities);
}

DECLDIR void Manager_Base_Frame(ECS::Manager_Base * mb)
{
	mb->Frame();
}
