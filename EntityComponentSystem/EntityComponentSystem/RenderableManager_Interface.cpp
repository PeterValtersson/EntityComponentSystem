#include <Managers\RenderableManager_Interface.h>
#include "RenderableManager.h"
DECLDIR_ECS ECS::RenderableManager_Interface * CreateRenderableManager(ECS::RenderableManager_InitializationInfo ii)
{
	return new ECS::RenderableManager(ii);
}
