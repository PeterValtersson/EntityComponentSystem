#include <Managers\RenderableManager_Interface.h>
#include "RenderableManager.h"
DECLDIR_ECS ECS::RenderableManager_Interface * CreateRenderableManager(ECS::RenderableManager_InitializationInfo ii)
{
	return new ECS::RenderableManager(ii);
}

DECLDIR_ECS void RenderableManager_Create_C(ECS::RenderableManager_Interface * rm, uint32_t entity)
{
	rm->Create(entity);
}
