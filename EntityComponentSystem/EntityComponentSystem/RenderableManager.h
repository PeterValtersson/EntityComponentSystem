#ifndef _ECS_RENDERABLE_MANAGER_H_
#define _ECS_RENDERABLE_MANAGER_H_
#include <Managers\RenderableManager_Interface.h>
#include <Utilities/Memory/Sofa.h>
namespace ECS
{
	class RenderableManager : public RenderableManager_Interface {
	public:
		RenderableManager( const RenderableManager_InitializationInfo& ii );
		~RenderableManager();

		/* Renderable manager methods */
		virtual void Create( Entity entity,
							 ResourceHandler::Resource mesh, ResourceHandler::Resource shader,
							 RenderableFlags render_flags = RenderableFlags::None,
							 MeshFlags mesh_flags = MeshFlags::Diffuse_Color )noexcept = 0;

		virtual void SetDefaultMesh( Utilities::GUID mesh ) = 0;
		virtual Utilities::GUID GetDefaultMesh()const noexcept = 0;

		virtual void SetDefaultShader( Utilities::GUID shader ) = 0;
		virtual Utilities::GUID GetDefaultShader()const noexcept = 0;

		virtual void ToggleWireframe( const Entity entity, bool wireFrame )noexcept = 0;
		virtual void ToggleWireframe( const Entity entity, uint8_t submesh, bool wireFrame )noexcept = 0;

		virtual void ToggleTransparency( const Entity entity, bool transparent )noexcept = 0;
		virtual void ToggleTransparency( const Entity entity, uint8_t submesh, bool transparent )noexcept = 0;

		virtual void ToggleShadow( const Entity entity, bool cast_shadow )noexcept = 0;
		virtual void ToggleShadow( const Entity entity, uint8_t submesh, bool cast_shadow )noexcept = 0;

		/* Manager base methods */
		virtual bool is_registered( Entity entity )const noexcept = 0;
		virtual void CreateFromResource( Entity entity, ResourceHandler::Resource resource ) = 0;
		virtual uint64_t GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer )const noexcept = 0;
		virtual void Destroy( Entity entity )noexcept = 0;
		virtual void DestroyMultiple( const Entity entities[], size_t numEntities )noexcept = 0;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept = 0;
		virtual void DestroyAll()noexcept = 0;

		// Visible/Invisible
		virtual void ToggleActive( Entity entity, bool active )noexcept = 0;
		virtual void ToggleActive( const Entity entities[], size_t numEntities, bool active )noexcept = 0;
		virtual void ToggleActive( const std::vector<Entity>& entities, bool active )noexcept = 0;

		virtual size_t GetNumberOfRegisteredEntities()const noexcept = 0;
		virtual void GetRegisteredEntities( Entity entities[], size_t numEntities )const noexcept = 0;
		virtual std::vector<Entity> GetRegisteredEntities()const noexcept = 0;

		virtual void Frame()noexcept = 0;

		virtual Utilities::GUID GetManagerType()const noexcept = 0;


		/* Memory base methods */
		virtual size_t get_memory_usage()const noexcept = 0;
		virtual void shrink_to_fit() = 0;
		virtual void write_to_stream( std::ostream& stream )const = 0;
		virtual void read_from_stream( std::istream& stream ) = 0;
	protected:
		/* Manager base methods */
		virtual void GarbageCollection()noexcept = 0;

		RenderableManager_InitializationInfo initInfo;
		uint32_t version = 0;

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

		std::vector<Entity> entitiesToSwitchMesh;
		std::vector<Entity> entitiesToSwitchShader;
	};
}
#endif
