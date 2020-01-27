#ifndef _ECS_TRANSFORM_MANAGER_H_
#define _ECS_TRANSFORM_MANAGER_H_
#include <Managers\TransformManager_Interface.h>
#include <Utilities/Memory/Sofa.h>
#include <DirectXMath.h>
#include <random>

using namespace DirectX;
namespace ECS
{
	class TransformManager : public TransformManager_Interface {
	public:
		TransformManager( ECS::TransformManager_Init_Info initInfo );
		~TransformManager();
		virtual void Create( Entity entity,
							 const Vector& position = Vector(),
							 const Vector& rotation = Vector(),
							 const Vector& scale = { 1.0f, 1.0f, 1.0f } )noexcept override;
		virtual void CreateMultiple( const std::vector<Entity>& entities,
									 const std::vector<Vector>& positions = {},
									 const std::vector<Vector>& rotations = {},
									 const std::vector<Vector>& scales = {} )noexcept override;

		virtual void BindChild( Entity parent, Entity child, TransformFlags flags = TransformFlags::KEEP_CURRENT_POSITION_ON_BIND | TransformFlags::INHERIT_ALL )noexcept override;
		virtual void UnbindParent( Entity entity )noexcept override;
		virtual void UnbindAllChildren( Entity entity )noexcept override;
		virtual Entity GetParent( Entity entity )const noexcept override;
		virtual size_t GetNumberOfChildren( Entity entity )const noexcept override;
		virtual void GetChildren( Entity parent, Entity children[] )const noexcept override;
		virtual std::vector<Entity> GetChildren( Entity parent )const noexcept override;

		virtual void SetPosition( Entity entity, const Vector& position )noexcept override;
		virtual Vector GetPosition( Entity entity )const noexcept override;

		virtual void SetRotation( Entity entity, const Vector& rotation )noexcept override;
		virtual Vector GetRotation( Entity entity )const noexcept  override;

		virtual void SetScale( Entity entity, const Vector& scale )noexcept override;
		virtual Vector GetScale( Entity entity )const noexcept override;

		virtual void SetTransform( Entity entity, const Matrix& transform )noexcept override;
		virtual Matrix GetTransform( Entity entity )noexcept override;

		virtual void RegisterTransformUser( Manager_TransformUser* tUser )noexcept override;
		virtual void UnregisterTransformUser( Manager_TransformUser* tUser )noexcept override;

		virtual void CreateFromResource( Entity entity, Resources::Resource resource ) override;
		virtual uint64_t GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer )const noexcept override;

		virtual bool is_registered( Entity entity )const noexcept override;
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

		virtual uint64_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit() override;
		virtual void write_to_stream( std::ostream& stream )const override;
		virtual void read_from_stream( std::istream& stream ) override;

		virtual std::vector<Matrix>& GetCleanedTransforms()const noexcept override;
	private:
		virtual void GarbageCollection()noexcept override;
		void UpdateDirtyEntities()noexcept;


		TransformManager_Init_Info initInfo;
		uint32_t version = 0;
		std::default_random_engine generator;
		std::vector<Manager_TransformUser*> transformUsers;
		struct Entries : public Utilities::Memory::SofA<
			Entity, Entity::Hasher,
			XMFLOAT3,
			XMFLOAT4,
			XMFLOAT3,
			XMFLOAT4X4,
			bool,
			TransformFlags,
			uint32_t,
			uint32_t,
			uint32_t> {
			enum Keys {
				Entity,
				Position,
				Rotation,
				Scale,
				Transform,
				Dirty,
				Flags,
				Child,
				Sibling,
				Parent
			};
		}entries;

	};
}
#endif
