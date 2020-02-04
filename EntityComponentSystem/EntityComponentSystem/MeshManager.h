#ifndef _ECS_RENDERABLE_MANAGER_H_
#define _ECS_RENDERABLE_MANAGER_H_
#include <Managers\MeshManager_Interface.h>
#include <Utilities/Memory/Sofa.h>
namespace ECS
{
	class MeshManager : public MeshManager_Interface {
	public:
		MeshManager( const MeshManager_InitializationInfo& ii );
		~MeshManager();

		/* Renderable manager methods */
		virtual void Create( Entity entity,
							 ResourceHandler::Resource mesh, ResourceHandler::Resource shader,
							 RenderableFlags render_flags = RenderableFlags::None,
							 MeshFlags mesh_flags = MeshFlags::Diffuse_Color )noexcept override;

		virtual void SetDefaultMesh( Utilities::GUID mesh ) override;
		virtual Utilities::GUID GetDefaultMesh()const noexcept override;

		virtual void SetDefaultShader( Utilities::GUID shader ) override;
		virtual Utilities::GUID GetDefaultShader()const noexcept override;

		virtual void ToggleWireframe( const Entity entity, bool wireFrame )noexcept override;
		virtual void ToggleWireframe( const Entity entity, uint8_t submesh, bool wireFrame )noexcept override;

		virtual void ToggleTransparency( const Entity entity, bool transparent )noexcept override;
		virtual void ToggleTransparency( const Entity entity, uint8_t submesh, bool transparent )noexcept override;

		virtual void ToggleShadow( const Entity entity, bool cast_shadow )noexcept override;
		virtual void ToggleShadow( const Entity entity, uint8_t submesh, bool cast_shadow )noexcept override;

		virtual void ToggleVisible( const Entity entity, bool visible )noexcept override;
		virtual void ToggleVisible( const Entity entity, uint8_t submesh, bool visible )noexcept override;

		virtual std::vector<std::string> GetSubmeshes( const Entity entity )const noexcept override;

		/* Manager base methods */
		virtual bool is_registered( Entity entity )const noexcept override;
		virtual void CreateFromResource( Entity entity, ResourceHandler::Resource resource ) override;
		virtual uint64_t GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer )const noexcept override;
		virtual void Destroy( Entity entity )noexcept override;
		virtual void DestroyMultiple( const Entity entities[], size_t numEntities )noexcept override;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept override;
		virtual void DestroyAll()noexcept override;

		virtual size_t GetNumberOfRegisteredEntities()const noexcept override;
		virtual void GetRegisteredEntities( Entity entities[], size_t numEntities )const noexcept override;
		virtual std::vector<Entity> GetRegisteredEntities()const noexcept override;

		virtual void Frame()noexcept override;

		virtual Utilities::GUID GetManagerType()const noexcept override;


		/* Memory base methods */
		virtual size_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit() override;
		virtual void write_to_stream( std::ostream& stream )const override;
		virtual void read_from_stream( std::istream& stream ) override;
	protected:
		/* Manager base methods */
		virtual void GarbageCollection()noexcept override;

		MeshManager_InitializationInfo initInfo;

		struct Entries : public Utilities::Memory::SofV<
			Entity, Entity::Hasher, // Entity
			ResourceHandler::Resource, // Mesh
			ResourceHandler::Resource, // Shader
			MeshInfo
		> {
			enum {
				Entity,
				Mesh,
				Shader,
				MeshInfo
			};
		}entries;
	};
}
#endif
