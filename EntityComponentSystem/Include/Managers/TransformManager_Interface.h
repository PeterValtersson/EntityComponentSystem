#ifndef _ECS_TRANSFORM_MANAGER_INTERFACE_H_
#define _ECS_TRANSFORM_MANAGER_INTERFACE_H_
#include <DLLExport.h>
#include <Managers\Manager_Base.h>

namespace ECS
{
	struct TransformManagerInitializationInfo
	{
		EntityManager_Interface* entityManager = nullptr;
		void* pNext = nullptr;
	};
	struct Vector
	{
		float x = 0.0f, y = 0.0f, z= 0.0f;
	};

	class TransformManager_Interface : public Manager_Base
	{
	public:
		virtual ~TransformManager_Interface() {};
		
		virtual void Create(Entity entity, const Vector& position = Vector(), const Vector& rotaiton = Vector(), const Vector& scale = Vector())noexcept = 0;
		
		virtual void SetPosition(Entity entity, const Vector& position)noexcept = 0;
		virtual Vector GetPosition(Entity entity)const noexcept = 0;

		virtual void SetRotation(Entity entity, const Vector& rotation)noexcept = 0;
		virtual Vector GetRotation(Entity entity)const noexcept = 0;

		virtual void SetScale(Entity entity, const Vector& scale)noexcept = 0;
		virtual Vector GetScale(Entity entity)const noexcept = 0;
	protected:
		TransformManager_Interface() {};
	};
}

DECLDIR ECS::TransformManager_Interface* TransformManager_CreateTransformManager_C(ECS::TransformManagerInitializationInfo initinfo);
DECLDIR void TransformManager_Create_C(ECS::TransformManager_Interface* tm, uint32_t entity, ECS::Vector position, ECS::Vector rotation, ECS::Vector scale);
DECLDIR void TransformManager_SetPosition_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector position);
DECLDIR ECS::Vector TransformManager_GetPosition_C(ECS::TransformManager_Interface*tm, uint32_t entity);
DECLDIR void TransformManager_SetRotation_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector rotation);
DECLDIR ECS::Vector TransformManager_GetRotation_C(ECS::TransformManager_Interface*tm, uint32_t entity);
DECLDIR void TransformManager_SetScale_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector scale);
DECLDIR ECS::Vector TransformManager_GetScale_C(ECS::TransformManager_Interface*tm, uint32_t entity);
#endif