//#include <Managers\MeshLibrary.h>
//#include <ResourceHandler\ResourceHandler_Interface.h>
//#include <stdint.h>
//
//struct MeshInfo
//{
//
//};
//std::unordered_map<Utilities::GUID, Utilities::GUID::Hasher, MeshInfo> meshes;
//
//
//ECS::MeshLibrary::MeshLibrary(Renderer::PipelineHandler_Interface* pipelineHandler)
//{
//	using namespace std::placeholders;
//	ResourceHandler::Type_Info info;
//
//	info.passthrough.Parse = std::bind(&MeshLibrary::Parse, this, _1, _2, _3, _4, _5); 
//	info.passthrough.DestroyParsedData = std::bind(&MeshLibrary::Destroy, this, _1, _2, _3);
//	ResourceHandler::Get()->AddType("StaticMesh", info);
//}
//
//
//ECS::MeshLibrary::~MeshLibrary()
//{
//}
//
//Utilities::Error ECS::MeshLibrary::Parse(uint32_t guid, void * data, uint64_t size, void ** parsedData, uint64_t * parsedSize)
//{
//	//pipelineHandler->CreateBuffer(guid, Renderer::Pipeline::Buffer::)
//
//
//
//	RETURN_SUCCESS;
//}
//
//Utilities::Error ECS::MeshLibrary::Destroy(uint32_t guid, void * data, uint64_t size)
//{
//	RETURN_SUCCESS;
//}
//
