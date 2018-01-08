#ifndef _ECS_RENDERABLE_MANAGER_INTERFACE_H_
#define _ECS_RENDERABLE_MANAGER_INTERFACE_H_
#include <DLLExport.h>
#include "Manager_Base.h"
#include <Graphics\Renderer_Interface.h> 
#include <Managers\TransformManager_Interface.h>
namespace ECS
{
	struct RenderableManager_InitializationInfo
	{
		Graphics::Renderer_Interface* renderer = nullptr;
		EntityManager_Interface* entityManager = nullptr;
		TransformManager_Interface* transformManager = nullptr;
	};
	class RenderableManager_Interface : public Manager_Base, public Manager_TransformUser
	{
	public:
		virtual ~RenderableManager_Interface() {};

		virtual void Create(Entity entity)noexcept = 0;
	protected:
		RenderableManager_Interface() {  };
	};
}
DECLDIR_ECS ECS::RenderableManager_Interface* CreateRenderableManager(ECS::RenderableManager_InitializationInfo ii);
DECLDIR_ECS void RenderableManager_Create_C(ECS::RenderableManager_Interface* rm, uint32_t entity);
#endif