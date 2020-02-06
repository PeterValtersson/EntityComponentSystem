#ifndef _ECS_CAMERA_MANAGER_INTERFACE_H_
#define _ECS_CAMERA_MANAGER_INTERFACE_H_

#pragma once
#include <Managers/Manager_Base.h>
#include <Managers/TransformManager_Interface.h>

namespace ECS
{
	struct Ray{
		Vector origin;
		Vector direction;
	};
	struct CameraManager_Init_Info {
		std::shared_ptr<EntityManager_Interface> entity_manager;
		std::shared_ptr<TransformManager_Interface> transform_manager;
		void* pNext = nullptr;
	};
	struct Camera_Create_Info {
		float aspectRatio = ( 800.0f / 640.0f );
		float fov = 1.570796f;
		float nearPlane = 0.01f;
		float farPlane = 100.0f;
		Vector position = { 0.f, 0.f, 0.f };
		Vector rotation = { 0.f, 0.f, 0.f };
	};

	class CameraManager_Interface : public Manager_Base{
	public:
		DECLDIR_ECS static std::shared_ptr<CameraManager_Interface> create_manager( const CameraManager_Init_Info& init_info );
		virtual ~CameraManager_Interface()
		{}
		virtual void Create( const Entity entity, const Camera_Create_Info& info )noexcept = 0;
		virtual void UpdateCamera( const Entity entity, const Camera_Create_Info& info )noexcept = 0;
		
		virtual Matrix GetView( const Entity entity )const noexcept = 0;
		virtual Matrix GetViewInv( const Entity entity )const noexcept = 0;
		virtual Matrix GetProjection( const Entity entity )const noexcept = 0;
		virtual Matrix GetViewProjection( const Entity entity )const noexcept = 0;
		virtual Ray WorldSpaceRayFromScreenPos( const Entity entity, int x, int y, int screenWidth, int screenHeight ) const noexcept = 0;
	};
}
#endif