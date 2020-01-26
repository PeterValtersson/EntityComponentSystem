#ifndef _ECS_CAMERA_MANAGER_INTERFACE_H_
#define _ECS_CAMERA_MANAGER_INTERFACE_H_

#pragma once
#include <Managers/Manager_Base.h>
#include <Managers/TransformManager_Interface.h>

namespace ECS
{
	struct CameraManager_Init_Info {
		std::shared_ptr<EntityManager_Interface> entity_manager;
		std::shared_ptr<TransformManager_Interface> transform_manager;
		void* pNext = nullptr;
	};
	struct Camera_Create_Info {
		float aspectRatio = ( 800.0f / 640.0f );
		float fov = 1.570796f;
		float nearPlane = 0.01f;
		float farPlance = 100.0f;
		Vector position = { 0.f, 0.f, 0.f };
		Vector rotation = { 0.f, 0.f, 0.f };
	};

	class CameraManager_Interface : Manager_Base{
	public:
		virtual void Create( const Entity& entity, const Camera_Create_Info& info ) = 0;
		virtual void UpdateCamera( const Entity& entity, const Camera_Create_Info& info ) = 0;
		virtual void Delete( const Entity& entitity ) = 0;

		virtual Matrix GetView( const Entity& entity )const = 0;
		virtual Matrix GetViewInv( const Entity& entity )const = 0;
		virtual Matrix GetProjection( const Entity& entity )const = 0;
		virtual Matrix GetViewProjection( const Entity& entity )const = 0;
		virtual void WorldSpaceRayFromScreenPos( int x, int y, int screenWidth, int screenHeight, Vector& origin, Vector& direction ) const = 0;
	};
}
#endif