#ifndef _ECS_RENDERABLE_MANAGER_H_
#define _ECS_RENDERABLE_MANAGER_H_
#include <Managers\RenderObjectManager_Interface.h>
#include <Utilities/Memory/Sofa.h>
#include "RenderObjectInstancing.h"

namespace ECS
{
	enum class MeshBitFlags{
		
	};
	class RenderObjectManager : public RenderObjectManager_Interface {
	public:
		RenderObjectManager( const RenderObjectManager_InitializationInfo& ii );
		~RenderObjectManager();

		/* Renderable manager methods */
		virtual void Create( Entity entity )noexcept override;

		virtual void Set_Shader( Entity entity, Utilities::GUID shader )noexcept override;
		virtual void Set_Mesh( Entity entity, Utilities::GUID mesh )noexcept override;

		virtual Utilities::GUID Get_Shader( Entity entity )noexcept override;
		virtual Utilities::GUID Get_Mesh( Entity entity )noexcept override;

		virtual void ToggleVisible( const Entity entity, bool visible )noexcept override;
		virtual void ToggleVisible( const Entity entity, uint8_t submesh, bool visible )noexcept override;

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

	private:
		/* Manager base methods */
		virtual void GarbageCollection()noexcept override;

		RenderObjectManager_InitializationInfo initInfo;
		std::default_random_engine generator;
		RenderObjectInstancing instancing;

		ResourceHandler::Resource default_mesh;
		ResourceHandler::Resource default_shader;

		struct Entries : public Utilities::Memory::SofV<
			Entity, Entity::Hasher, // Entity
			ResourceHandler::Resource, // Mesh
			ResourceHandler::Resource, // Shader
			Renderer::RenderGroup,
			bool // Visible
		> {
			enum {
				Entity,
				Mesh,
				Shader,
				RenderGroup,
				Visible
			};
		}entries;
	};
}
#endif