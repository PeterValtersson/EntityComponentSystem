#include <Managers\TransformManager_Interface.h>
#include "TransformManager.h"
DECLDIR_ECS ECS::TransformManager_Interface * TransformManager_CreateTransformManager_C(ECS::TransformManagerInitializationInfo ii)
{
	return new ECS::TransformManager(ii);
}

DECLDIR_ECS void TransformManager_Create_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector position, ECS::Vector rotation, ECS::Vector scale)
{
	tm->Create(entity, position, rotation, scale);
}

DECLDIR_ECS void TransformManager_BindChild_C(ECS::TransformManager_Interface * tm, uint32_t parent, uint32_t child, uint8_t flags)
{
	tm->BindChild(parent, child, ECS::TransformFlags(flags));
}


DECLDIR_ECS void TransformManager_UnbindParent_C(ECS::TransformManager_Interface * tm, uint32_t child, uint8_t flags)
{
	tm->UnbindParent(child, ECS::TransformFlags(flags));
}

DECLDIR_ECS void TransformManager_UnbindAllChildren_C(ECS::TransformManager_Interface * tm, uint32_t entity, uint8_t flags)
{
	tm->UnbindAllChildren(entity, ECS::TransformFlags(flags));
}

DECLDIR_ECS uint32_t TransformManager_GetParent_C(ECS::TransformManager_Interface * tm, uint32_t entity)
{
	return tm->GetParent(entity);
}

DECLDIR_ECS uint32_t TransformManager_GetNumberOfChildren_C(ECS::TransformManager_Interface * tm, uint32_t entity)
{
	return tm->GetNumberOfChildren(entity);
}

DECLDIR_ECS void TransformManager_GetChildren_C(ECS::TransformManager_Interface * tm, uint32_t entity, uint32_t * children)
{
	tm->GetChildren(entity, (ECS::Entity*)children);
}

DECLDIR_ECS void TransformManager_SetPosition_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector position)
{
	tm->SetPosition(entity, position);
}

DECLDIR_ECS ECS::Vector TransformManager_GetScale_C(ECS::TransformManager_Interface * tm, uint32_t entity)
{
	return tm->GetScale(entity);
}

DECLDIR_ECS void TransformManager_SetTransform_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Matrix transform)
{
	tm->SetTransform(entity, transform);
}

DECLDIR_ECS ECS::Matrix TransformManager_GetTransform_C(ECS::TransformManager_Interface * tm, uint32_t entity)
{
	return tm->GetTransform(entity);
}

DECLDIR_ECS void TransformManager_RegisterTransformUser(ECS::TransformManager_Interface * tm, ECS::Manager_TransformUser * tu)
{
	tm->RegisterTransformUser(tu);
}

DECLDIR_ECS void TransformManager_UnregisterTransformUser(ECS::TransformManager_Interface * tm, ECS::Manager_TransformUser * tu)
{
	tm->UnregisterTransformUser(tu);
}

DECLDIR_ECS void TransformManager_GetPosition_NoReturn_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector * position)
{
	*position = tm->GetPosition(entity);
}

DECLDIR_ECS void TransformManager_GetRotation_NoReturn_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector * rotation)
{
	*rotation = tm->GetRotation(entity);
}

DECLDIR_ECS void TransformManager_GetScale_NoReturn_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector * scale)
{
	*scale = tm->GetScale(entity);
}

DECLDIR_ECS ECS::Vector TransformManager_GetRotation_C(ECS::TransformManager_Interface * tm, uint32_t entity)
{
	return tm->GetRotation(entity);
}

DECLDIR_ECS void TransformManager_SetScale_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector scale)
{
	tm->SetScale(entity, scale);
}


DECLDIR_ECS ECS::Vector TransformManager_GetPosition_C(ECS::TransformManager_Interface * tm, uint32_t entity)
{
	return tm->GetPosition(entity);
}

DECLDIR_ECS void TransformManager_SetRotation_C(ECS::TransformManager_Interface * tm, uint32_t entity, ECS::Vector rotation)
{
	tm->SetRotation(entity, rotation);
}
