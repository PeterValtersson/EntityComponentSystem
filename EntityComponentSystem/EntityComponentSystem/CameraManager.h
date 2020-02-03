#ifndef _ECS_CAMERA_MANAGER_H_
#define _ECS_CAMERA_MANAGER_H_

#pragma once
#include <Managers/CameraManager_Interface.h>
#include <Utilities/Memory/Sofa.h>
#include <DirectXMath.h>
namespace ECS
{
	class CameraManager : public CameraManager_Interface, public Manager_TransformUser {
	public:
		CameraManager( const CameraManager_Init_Info& ii );
		virtual ~CameraManager()noexcept;
		/* Transform User methods */
		virtual void UpdateEntityTransforms( const Matrix transforms[], const Entity entities[], size_t numEntities )noexcept;

		/* Camera manager methods*/
		virtual void Create( const Entity entity, const Camera_Create_Info& info )noexcept override;
		virtual void UpdateCamera( const Entity entity, const Camera_Create_Info& info )noexcept override;

		virtual Matrix GetView( const Entity entity )const noexcept override;
		virtual Matrix GetViewInv( const Entity entity )const noexcept override;
		virtual Matrix GetProjection( const Entity entity )const noexcept override;
		virtual Matrix GetViewProjection( const Entity entity )const noexcept override;
		virtual Ray WorldSpaceRayFromScreenPos( const Entity entity, int x, int y, int screenWidth, int screenHeight ) const noexcept override;


		/* Manager Base Methods*/
		virtual bool is_registered( Entity entity )const noexcept override;
		virtual void CreateFromResource( Entity entity, ResourceHandler::Resource resource ) override;
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

	private:
		/* Manager Base Methods */
		virtual void GarbageCollection()noexcept override;

	private:
		std::default_random_engine generator;
		CameraManager_Init_Info init_info;
		const Matrix* cleaned_transforms;

		struct Entries : public Utilities::Memory::SofA <
			Entity, Entity::Hasher,
			size_t, // Index to cleaned transform != ~0 if dirty.
			float, // FoV
			float, // Aspect Ratio
			float, // Near plane
			float, // Far plane
			DirectX::XMFLOAT4X4 // View
		>{
			enum Keys{
				Entity,
				Dirty,
				FoV,
				AspectRatio,
				NearPlane,
				FarPlane,
				View
			};
		}entries;
	};
}
#endif
