#ifndef _ECS_CAMERA_MANAGER_H_
#define _ECS_CAMERA_MANAGER_H_

#pragma once
#include <Managers/CameraManager_Interface.h>

namespace ECS
{
	class CameraManager : public CameraManager_Interface {
		/* Camera manager methods*/
		virtual void Create( const Entity& entity, const Camera_Create_Info& info ) override;
		virtual void UpdateCamera( const Entity& entity, const Camera_Create_Info& info ) override;
		virtual void Delete( const Entity& entitity ) override;

		virtual Matrix GetView( const Entity& entity )const override;
		virtual Matrix GetViewInv( const Entity& entity )const override;
		virtual Matrix GetProjection( const Entity& entity )const override;
		virtual Matrix GetViewProjection( const Entity& entity )const override;
		virtual void WorldSpaceRayFromScreenPos( int x, int y, int screenWidth, int screenHeight, Vector& origin, Vector& direction ) const override;


		/* Manager Base Methods*/
		virtual bool is_registered( Entity entity )const noexcept override;
		virtual void CreateFromResource( Entity entity, Resources::Resource resource ) override;
		virtual uint64_t GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer )const noexcept override;
		virtual void Destroy( Entity entity )noexcept override;
		virtual void DestroyMultiple( const Entity entities[], size_t numEntities )noexcept override;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept override;
		virtual void DestroyAll()noexcept override;

		virtual void ToggleActive( Entity entity, bool active )noexcept override;
		virtual void ToggleActive( const Entity entities[], size_t numEntities, bool active )noexcept override;
		virtual void ToggleActive( const std::vector<Entity>& entities, bool active )noexcept override;

		virtual size_t GetNumberOfRegisteredEntities()const noexcept override;
		virtual void GetRegisteredEntities( Entity entities[], size_t numEntities )const noexcept override;
		virtual std::vector<Entity> GetRegisteredEntities()const noexcept override;

		virtual void Frame()noexcept override;

		virtual Utilities::GUID GetManagerType()const noexcept override;

		/* Memory base methods*/
		virtual size_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit() override;
		virtual void write_to_stream( std::ostream& stream )const override;
		virtual void read_from_stream( std::istream& stream ) override;
	};
}
#endif
