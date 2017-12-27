#ifndef _ECS_TRANSFORM_MANAGER_INTERFACE_H_
#define _ECS_TRANSFORM_MANAGER_INTERFACE_H_
#include <DLLExport.h>
#include <Managers\Manager_Base.h>
#define ENUM_FLAG_OPERATOR(T,X) inline T operator X (T lhs, T rhs) { return (T) (static_cast<std::underlying_type_t <T>>(lhs) X static_cast<std::underlying_type_t <T>>(rhs)); } 
#define ENUM_FLAG_OPERATOR2(T,X) inline void operator X= (T& lhs, T rhs) { lhs = (T)(static_cast<std::underlying_type_t <T>>(lhs) X static_cast<std::underlying_type_t <T>>(rhs)); } 
#define ENUM_FLAGS(T) \
inline T operator ~ (T t) { return (T) (~static_cast<std::underlying_type_t <T>>(t)); } \
inline bool operator & (T lhs, T rhs) { return (static_cast<std::underlying_type_t <T>>(lhs) & static_cast<std::underlying_type_t <T>>(rhs));  } \
ENUM_FLAG_OPERATOR2(T,|) \
ENUM_FLAG_OPERATOR2(T,&) \
ENUM_FLAG_OPERATOR(T,|) \
ENUM_FLAG_OPERATOR(T,^)
//enum class T
//ENUM_FLAG_OPERATOR(T,&)

namespace ECS
{
	enum class TransformFlags : uint8_t
	{
		INHERIT_TRANSLATION = 1 << 0,
		INHERIT_SCALE = 1 << 1,
		INHERIT_ROTATION = 1 << 2,
		TRANSLATE_TO_PARENT = 1 << 3,
		INHERIT_ALL = INHERIT_TRANSLATION | INHERIT_SCALE | INHERIT_ROTATION,
	};
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
		
		virtual void Create(Entity entity,
			const Vector& position = Vector(), 
			const Vector& rotaiton = Vector(), 
			const Vector& scale = Vector())noexcept = 0;
		
		virtual void BindChild(Entity parent, Entity child, TransformFlags flags)noexcept = 0;
		
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

ENUM_FLAGS(ECS::TransformFlags);

DECLDIR ECS::TransformManager_Interface* TransformManager_CreateTransformManager_C(ECS::TransformManagerInitializationInfo initinfo);
DECLDIR void TransformManager_Create_C(ECS::TransformManager_Interface* tm, uint32_t entity, ECS::Vector position, ECS::Vector rotation, ECS::Vector scale);
DECLDIR void TransformManager_BindChild_C(ECS::TransformManager_Interface*tm, uint32_t parent, uint32_t child, uint8_t flags);
DECLDIR void TransformManager_SetPosition_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector position);
DECLDIR ECS::Vector TransformManager_GetPosition_C(ECS::TransformManager_Interface*tm, uint32_t entity);
DECLDIR void TransformManager_SetRotation_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector rotation);
DECLDIR ECS::Vector TransformManager_GetRotation_C(ECS::TransformManager_Interface*tm, uint32_t entity);
DECLDIR void TransformManager_SetScale_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector scale);
DECLDIR ECS::Vector TransformManager_GetScale_C(ECS::TransformManager_Interface*tm, uint32_t entity);
DECLDIR void TransformManager_SetTransform_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Matrix transform);
DECLDIR ECS::Matrix TransformManager_GetTransform_C(ECS::TransformManager_Interface*tm, uint32_t entity);
DECLDIR void TransformManager_RegisterTransformUser(ECS::TransformManager_Interface* tm, ECS::Manager_TransformUser* tu);
DECLDIR void TransformManager_UnregisterTransformUser(ECS::TransformManager_Interface* tm, ECS::Manager_TransformUser* tu);
#endif