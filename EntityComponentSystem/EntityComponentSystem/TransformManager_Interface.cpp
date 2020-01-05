#include <Managers\TransformManager_Interface.h>
#include "TransformManager.h"
//DECLDIR_ECS_C ECS::TransformManager_Interface * TransformManager_CreateTransformManager_C(ECS::TransformManager_Init_Info ii)
//{
//	return new ECS::TransformManager(ii);
//}
//
//DECLDIR_ECS_C void TransformManager_Create_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector position, ECS::Vector rotation, ECS::Vector scale)
//{
//	tm->Create(entity, position, rotation, scale);
//}
//
//DECLDIR_ECS_C void TransformManager_BindChild_C(ECS::TransformManager_Interface * tm, uint32_t parent, uint32_t child, uint8_t flags)
//{
//	tm->BindChild(parent, child, ECS::TransformFlags(flags));
//}
//
//
//DECLDIR_ECS_C void TransformManager_UnbindParent_C(ECS::TransformManager_Interface * tm, uint32_t child, uint8_t flags)
//{
//	tm->UnbindParent(child, ECS::TransformFlags(flags));
//}
//
//DECLDIR_ECS_C void TransformManager_UnbindAllChildren_C(ECS::TransformManager_Interface * tm, uint32_t entity, uint8_t flags)
//{
//	tm->UnbindAllChildren(entity, ECS::TransformFlags(flags));
//}
//
//DECLDIR_ECS_C uint32_t TransformManager_GetParent_C(ECS::TransformManager_Interface * tm, uint32_t entity)
//{
//	return tm->GetParent(entity);
//}
//
//DECLDIR_ECS_C uint32_t TransformManager_GetNumberOfChildren_C(ECS::TransformManager_Interface * tm, uint32_t entity)
//{
//	return tm->GetNumberOfChildren(entity);
//}
//
//DECLDIR_ECS_C void TransformManager_GetChildren_C(ECS::TransformManager_Interface * tm, uint32_t entity, uint32_t * children)
//{
//	tm->GetChildren(entity, (ECS::Entity*)children);
//}
//
//DECLDIR_ECS_C void TransformManager_SetPosition_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector position)
//{
//	tm->SetPosition(entity, position);
//}
//
//DECLDIR_ECS_C ECS::Vector TransformManager_GetScale_C(ECS::TransformManager_Interface * tm, uint32_t entity)
//{
//	return tm->GetScale(entity);
//}
//
//DECLDIR_ECS_C void TransformManager_SetTransform_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Matrix transform)
//{
//	tm->SetTransform(entity, transform);
//}
//
//DECLDIR_ECS_C ECS::Matrix TransformManager_GetTransform_C(ECS::TransformManager_Interface * tm, uint32_t entity)
//{
//	return tm->GetTransform(entity);
//}
//
//DECLDIR_ECS_C void TransformManager_RegisterTransformUser(ECS::TransformManager_Interface * tm, ECS::Manager_TransformUser * tu)
//{
//	tm->RegisterTransformUser(tu);
//}
//
//DECLDIR_ECS_C void TransformManager_UnregisterTransformUser(ECS::TransformManager_Interface * tm, ECS::Manager_TransformUser * tu)
//{
//	tm->UnregisterTransformUser(tu);
//}
//
//DECLDIR_ECS_C void TransformManager_GetPosition_NoReturn_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector * position)
//{
//	*position = tm->GetPosition(entity);
//}
//
//DECLDIR_ECS_C void TransformManager_GetRotation_NoReturn_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector * rotation)
//{
//	*rotation = tm->GetRotation(entity);
//}
//
//DECLDIR_ECS_C void TransformManager_GetScale_NoReturn_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector * scale)
//{
//	*scale = tm->GetScale(entity);
//}
//
//DECLDIR_ECS_C ECS::Vector TransformManager_GetRotation_C(ECS::TransformManager_Interface * tm, uint32_t entity)
//{
//	return tm->GetRotation(entity);
//}
//
//DECLDIR_ECS_C void TransformManager_SetScale_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector scale)
//{
//	tm->SetScale(entity, scale);
//}
//
//
//DECLDIR_ECS_C ECS::Vector TransformManager_GetPosition_C(ECS::TransformManager_Interface * tm, uint32_t entity)
//{
//	return tm->GetPosition(entity);
//}
//
//DECLDIR_ECS_C void TransformManager_SetRotation_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector rotation)
//{
//	tm->SetRotation(entity, rotation);
//}

std::shared_ptr<ECS::TransformManager_Interface> ECS::TransformManager_Interface::create( TransformManager_Init_Info init_info )
{
	return std::make_shared<TransformManager>( init_info );
}
