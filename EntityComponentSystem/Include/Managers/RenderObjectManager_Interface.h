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
	struct RenderObjectManager_InitializationInfo{
		std::shared_ptr<Renderer::Renderer_Interface> renderer = nullptr;
		std::shared_ptr<EntityManager_Interface> entityManager = nullptr;
		std::shared_ptr<TransformManager_Interface> transformManager = nullptr;
		void* pNext;
	};

	enum class MeshFlags : uint32_t{
		None,
		Skinned,
		Transparent,
		Cast_Shadow
	};

	ENUM_FLAGS( ECS::MeshFlags );

	typedef char SubMeshName[256];

	struct MeshInfo{
		static constexpr uint8_t max_sub_mesh_count = 10;
		uint8_t sub_mesh_count = 0;
		uint32_t vertex_start[max_sub_mesh_count];
		uint32_t vertex_count[max_sub_mesh_count];
		MeshFlags flags[max_sub_mesh_count];
		SubMeshName name[max_sub_mesh_count];
	};

	enum class RenderableFlags{
		None,
	};

	struct RenderObjectManager_ResourceInfo{
		Utilities::GUID mesh;
		Utilities::GUID shader[MeshInfo::max_sub_mesh_count];
		RenderableFlags flags;
	};
	class RenderObjectManager_Interface : public Manager_Base, public Manager_TransformUser{
	public:
		DECLDIR_ECS static std::shared_ptr<RenderObjectManager_Interface> Create_Manager( const RenderObjectManager_ResourceInfo& info );

		virtual ~RenderObjectManager_Interface()
		{};

		virtual void Create( Entity entity )noexcept = 0;

		virtual void Edit_Pipeline( Entity entity, const std::function<void( Renderer::Pipeline::Pipeline_Mutable )>& callback )noexcept = 0;
	protected:
		RenderObjectManager_Interface()
		{};
	};
}
//DECLDIR_ECS_C ECS::RenderObjectManager_Interface* RenderObjectManager_CreateRenderObjectManager(ECS::RenderObjectManager_InitializationInfo ii);
//DECLDIR_ECS_C void RenderObjectManager_Create_C(ECS::RenderObjectManager_Interface* rm, uint32_t entity, 
//	const char* mesh, const char* meshType,
//	const char* defaultMesh, const char* defaultMeshType,
//	const char* shader, const char* shaderType,
//	const char* defaultShader, const char* defaultShaderType);
#endif