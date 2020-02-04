#include <Managers/MeshManager_Interface.h>

//#include <Managers\MeshManager_Interface.h>
//#include "MeshManager.h"
//DECLDIR_ECS_C ECS::MeshManager_Interface * MeshManager_CreateMeshManager(ECS::MeshManager_InitializationInfo ii)
//{
//	return new ECS::MeshManager(ii);
//}
//
//DECLDIR_ECS_C void MeshManager_Create_C(ECS::MeshManager_Interface * rm, uint32_t entity,
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

DECLDIR_ECS std::shared_ptr<MeshManager_Interface> ECS::MeshManager_Interface::Create_Manager( const MeshManager_ResourceInfo& info )
{
	return DECLDIR_ECS std::shared_ptr<MeshManager_Interface>();
}
