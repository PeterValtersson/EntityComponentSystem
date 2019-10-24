#include <Managers\RenderableManager_Interface.h>
#include "RenderableManager.h"
DECLDIR_ECS_C ECS::RenderableManager_Interface * RenderableManager_CreateRenderableManager(ECS::RenderableManager_InitializationInfo ii)
{
	return new ECS::RenderableManager(ii);
}

DECLDIR_ECS_C void RenderableManager_Create_C(ECS::RenderableManager_Interface * rm, uint32_t entity,
	const char* mesh, const char* meshType,
	const char* defaultMesh, const char* defaultMeshType,
	const char* shader, const char* shaderType,
	const char* defaultShader, const char* defaultShaderType)
{
	rm->Create(entity,
		ResourceHandler::Resource(std::string(mesh), std::string(meshType)),
		ResourceHandler::Resource(std::string(defaultMesh), std::string(defaultMeshType)),
		ResourceHandler::Resource(std::string(shader), std::string(shaderType)),
		ResourceHandler::Resource(std::string(defaultShader), std::string(defaultShaderType)));
}
