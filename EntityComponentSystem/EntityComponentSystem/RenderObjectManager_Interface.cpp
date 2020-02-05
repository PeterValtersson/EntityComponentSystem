#include <Managers/RenderObjectManager_Interface.h>

//#include <Managers\RenderObjectManager_Interface.h>
//#include "RenderObjectManager.h"
//DECLDIR_ECS_C ECS::RenderObjectManager_Interface * RenderObjectManager_CreateRenderObjectManager(ECS::RenderObjectManager_InitializationInfo ii)
//{
//	return new ECS::RenderObjectManager(ii);
//}
//
//DECLDIR_ECS_C void RenderObjectManager_Create_C(ECS::RenderObjectManager_Interface * rm, uint32_t entity,
//	const char* mesh, const char* meshType,
//	const char* defaultMesh, const char* defaultMeshType,
//	const char* shader, const char* shaderType,
//	const char* defaultShader, const char* defaultShaderType)
//{
//	rm->Create(entity,
//		ResourceHandler::Resource(std::string(mesh), std::string(meshType)),
//		ResourceHandler::Resource(std::string(defaultMesh), std::string(defaultMeshType)),
//		ResourceHandler::Resource(std::string(shader), std::string(shaderType)),
//		ResourceHandler::Resource(std::string(defaultShader), std::string(defaultShaderType)));
//}

DECLDIR_ECS std::shared_ptr<RenderObjectManager_Interface> ECS::RenderObjectManager_Interface::Create_Manager( const RenderObjectManager_ResourceInfo& info )
{
	return DECLDIR_ECS std::shared_ptr<RenderObjectManager_Interface>();
}
