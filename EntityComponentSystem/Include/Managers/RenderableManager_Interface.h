#ifndef _ECS_RENDERABLE_MANAGER_INTERFACE_H_
#define _ECS_RENDERABLE_MANAGER_INTERFACE_H_
#include <DLLExport.h>
#include "Manager_Base.h"
#include <Graphics/Renderer_Interface.h>
#include <Managers\TransformManager_Interface.h>
#include <Utilities/Flags.h>
#include <vector>
#include <string>

namespace ECS
{
	struct RenderableManager_InitializationInfo
	{
		std::shared_ptr<Renderer::Renderer_Interface> renderer = nullptr;
		std::shared_ptr<EntityManager_Interface> entityManager = nullptr;
		std::shared_ptr<TransformManager_Interface> transformManager = nullptr;
		void *pNext;
	};

	enum class MeshFlags : uint32_t{
		None,
		Diffuse_Color,
		Texture,
		Normal,
		Tangent,
		Skinned,
		Transparent,
		Cast_Shadow
	};

	ENUM_FLAGS( ECS::MeshFlags );

	struct MeshInfo{
		static constexpr uint8_t max_sub_mesh_count = 10;
		uint8_t sub_mesh_count;
		size_t vertex_start[max_sub_mesh_count];
		size_t vertex_count[max_sub_mesh_count];
		MeshFlags flags[max_sub_mesh_count];
	};

	enum class RenderableFlags{
		None,
	};

	struct RenderableManager_ResourceInfo
	{
		Utilities::GUID mesh;
		Utilities::GUID shader[MeshInfo::max_sub_mesh_count];
		RenderableFlags flags;
	};
	class RenderableManager_Interface : public Manager_Base, public Manager_TransformUser
	{
	public:
		virtual ~RenderableManager_Interface() {};

		virtual void Create(Entity entity, 
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

		virtual void ToggleVisible( const Entity entity, bool visible )noexcept = 0;
		virtual void ToggleVisible( const Entity entity, uint8_t submesh, bool visible )noexcept = 0;

		virtual std::vector<std::string> GetSubmeshes( const Entity entity )const noexcept = 0;

	protected:
		RenderableManager_Interface() {  };
	};
}
//DECLDIR_ECS_C ECS::RenderableManager_Interface* RenderableManager_CreateRenderableManager(ECS::RenderableManager_InitializationInfo ii);
//DECLDIR_ECS_C void RenderableManager_Create_C(ECS::RenderableManager_Interface* rm, uint32_t entity, 
//	const char* mesh, const char* meshType,
//	const char* defaultMesh, const char* defaultMeshType,
//	const char* shader, const char* shaderType,
//	const char* defaultShader, const char* defaultShaderType);
#endif