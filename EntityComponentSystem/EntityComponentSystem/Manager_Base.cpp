//#include "Manager_Base.h"
//DECLDIR_ECS_C bool Manager_Base_IsRegistered_C(ECS::Manager_Base * mb, uint32_t entity)
//{
//	return mb->is_registered(entity);
//}
//DECLDIR_ECS_C void Manager_Base_Destroy_C(ECS::Manager_Base * mb, uint32_t entity)
//{
//	mb->Destroy(entity);
//}
//
//DECLDIR_ECS_C void Manager_Base_DestroyEntities_C(ECS::Manager_Base * mb, const uint32_t entities[], size_t numEntities)
//{
//	mb->DestroyEntities((const ECS::Entity*)entities, numEntities);
//}
//
//DECLDIR_ECS_C void Manager_Base_DestroyAll_C(ECS::Manager_Base * mb)
//{
//	mb->DestroyAll();
//}
//
//DECLDIR_ECS_C void Manager_Base_ToggleActive_C(ECS::Manager_Base * mb, uint32_t entity, bool active)
//{
//	mb->ToggleActive(entity, active);
//}
//
//DECLDIR_ECS_C void Manager_Base_ToggleActiveMultiple_C(ECS::Manager_Base * mb, const uint32_t entities[], size_t numEntities, bool active)
//{
//	mb->ToggleActive((ECS::Entity*)entities, numEntities, active);
//}
//
//DECLDIR_ECS_C uint32_t Manager_Base_GetNumberOfRegisteredEntities_C(ECS::Manager_Base * mb)
//{
//	return mb->GetNumberOfRegisteredEntities();
//}
//
//DECLDIR_ECS_C void Manager_Base_GetRegisteredEntities_C(ECS::Manager_Base * mb, uint32_t * entities, size_t numEntities)
//{
//	mb->GetRegisteredEntities((ECS::Entity*)entities, numEntities);
//}
//#include <sstream>
//DECLDIR_ECS_C void Manager_Base_Frame_C(ECS::Manager_Base * mb)
//{
//	mb->Frame();
//}
//
//DECLDIR_ECS_C void Manager_Base_CreateFromResource_C(ECS::Manager_Base * mb, uint32_t entity, const char * guid, const char * type)
//{
//	 mb->CreateFromResource(entity, ResourceHandler::Resource(std::string(guid), std::string(type)));
//}
//
//DECLDIR_ECS_C void Manager_Base_CreateFromResourceG_C(ECS::Manager_Base * mb, uint32_t entity, uint32_t guid, uint32_t type)
//{
//	mb->CreateFromResource(entity, ResourceHandler::Resource(guid, type));
//}
//
//DECLDIR_ECS_C Utilities::Error Manager_Base_WriteComponent_C(ECS::Manager_Base * mb, ResourceHandler::FileSystem_Interface * li, uint32_t entity, const char * guid, const char * type)
//{	
//	std::function<bool(std::ostream* file)> writer;
//	auto size = mb->GetDataWriter(entity, writer);
//	if (size == 0)
//		RETURN_ERROR("Could not fetch data writer");
//	auto res = li->CreateFromCallback(guid, type, writer);
//	if (res.hash == "File already exists"_hash)
//		res =  li->WriteFromCallback(std::string(guid), std::string(type), size, writer);
//	if (res.hash == "Success"_hash)
//		ResourceHandler::Resource(std::string(guid), std::string(type), true);
//	return res;
//}
//
//DECLDIR_ECS_C uint32_t Manager_Base_GetManagerType_C(ECS::Manager_Base * mb)
//{
//	return mb->GetManagerType();
//}
