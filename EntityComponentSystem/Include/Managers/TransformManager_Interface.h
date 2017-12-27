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
	struct Matrix
	{
		float m[4][4] = 
		{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
	};

	class Manager_TransformUser
	{
	public:
		virtual void UpdateEntityTransforms(const Matrix transforms[], const Entity entities[], uint32_t numEntities)noexcept = 0;
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

		virtual void SetTransform(Entity entity, const Matrix& transform)noexcept = 0;
		virtual Matrix GetTransform(Entity entity)noexcept = 0;

		virtual void RegisterTransformUser(Manager_TransformUser* tUser)noexcept = 0;
		virtual void UnregisterTransformUser(Manager_TransformUser* tUser)noexcept = 0;
		
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
DECLDIR void TransformManager_SetTransform_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Matrix transform);
DECLDIR ECS::Matrix TransformManager_GetTransform_C(ECS::TransformManager_Interface*tm, uint32_t entity);
#endif