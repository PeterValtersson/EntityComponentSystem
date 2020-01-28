#ifndef _ECS_TRANSFORM_MANAGER_INTERFACE_H_
#define _ECS_TRANSFORM_MANAGER_INTERFACE_H_
#include <DLLExport.h>
#include <Managers\Manager_Base.h>
#include <Utilities/Flags.h>
#include <algorithm>

namespace ECS
{
	enum class TransformFlags : uint8_t {
		NONE = 0,
		INHERIT_TRANSLATION = 1 << 0,
		INHERIT_SCALE = 1 << 1,
		INHERIT_ROTATION = 1 << 2,
		KEEP_CURRENT_POSITION_ON_BIND = 1 << 3,
		INHERIT_ALL = INHERIT_TRANSLATION | INHERIT_SCALE | INHERIT_ROTATION,
	};
	ENUM_FLAGS( ECS::TransformFlags );
	struct TransformManager_Init_Info {
		std::shared_ptr<EntityManager_Interface> entityManager;
		void* pNext = nullptr;
	};
	struct Vector {
		float x = 0.0f, y = 0.0f, z = 0.0f;
	};
	inline bool operator==( const Vector& l, const Vector& r )
	{
		return ( std::abs( l.x - r.x ) < 0.001f ) && ( std::abs( l.y - r.y ) < 0.001f ) && ( std::abs( l.z - r.z ) < 0.001f );
	}
	struct Matrix {
		union{
			struct{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	};
	inline bool operator==( const Matrix& a, const Matrix& b )
	{
		for ( int y = 0; y < 4; y++ )
			for ( int x = 0; x < 4; x++ )
				if ( std::abs( a.m[y][x] - b.m[y][x] ) > 0.001f )
					return false;
		return true;
	}
	class Manager_TransformUser {
	public:
		virtual void UpdateEntityTransforms( const Matrix transforms[], const Entity entities[], size_t numEntities )noexcept = 0;
	};

	class TransformManager_Interface : public Manager_Base {
	public:
		DECLDIR_ECS static std::shared_ptr<TransformManager_Interface> create_manager( TransformManager_Init_Info init_info );



		virtual ~TransformManager_Interface() {};

		virtual void Create( Entity entity,
							 const Vector& position = Vector(),
							 const Vector& rotation = Vector(),
							 const Vector& scale = { 1.0f, 1.0f, 1.0f } )noexcept = 0;
		virtual void CreateMultiple( const std::vector<Entity>& entities,
									 const std::vector<Vector>& positions = {},
									 const std::vector<Vector>& rotations = {},
									 const std::vector<Vector>& scales = {} )noexcept = 0;

		virtual void BindChild( Entity parent, Entity child, TransformFlags flags = TransformFlags::KEEP_CURRENT_POSITION_ON_BIND | TransformFlags::INHERIT_ALL )noexcept = 0;
		virtual void UnbindParent( Entity entity )noexcept = 0;
		virtual void UnbindAllChildren( Entity entity )noexcept = 0;
		virtual Entity GetParent( Entity entity )const noexcept = 0;
		virtual size_t GetNumberOfChildren( Entity entity )const noexcept = 0;
		virtual void GetChildren( Entity parent, Entity children[] )const noexcept = 0;
		virtual std::vector<Entity> GetChildren( Entity parent )const noexcept = 0;

		virtual void SetPosition( Entity entity, const Vector& position )noexcept = 0;
		virtual Vector GetPosition( Entity entity )const noexcept = 0;

		virtual void SetRotation( Entity entity, const Vector& rotation )noexcept = 0;
		virtual Vector GetRotation( Entity entity )const noexcept = 0;

		virtual void SetScale( Entity entity, const Vector& scale )noexcept = 0;
		virtual Vector GetScale( Entity entity )const noexcept = 0;

		virtual void SetTransform( Entity entity, const Matrix& transform )noexcept = 0;
		virtual Matrix GetTransform( Entity entity )noexcept = 0;

		virtual void RegisterTransformUser( Manager_TransformUser* tUser )noexcept = 0;
		virtual void UnregisterTransformUser( Manager_TransformUser* tUser )noexcept = 0;

	protected:
		TransformManager_Interface() {};
	};
}


//DECLDIR_ECS_C ECS::TransformManager_Interface* TransformManager_CreateTransformManager_C(ECS::TransformManager_Init_Info init_info);
//DECLDIR_ECS_C void TransformManager_Create_C(ECS::TransformManager_Interface* tm, uint32_t entity, ECS::Vector position, ECS::Vector rotation, ECS::Vector scale);
//DECLDIR_ECS_C void TransformManager_BindChild_C(ECS::TransformManager_Interface*tm, uint32_t parent, uint32_t child, uint8_t flags);
//DECLDIR_ECS_C void TransformManager_UnbindParent_C(ECS::TransformManager_Interface*tm, uint32_t child, uint8_t flags);
//DECLDIR_ECS_C void TransformManager_UnbindAllChildren_C(ECS::TransformManager_Interface*tm, uint32_t entity, uint8_t flags);
//DECLDIR_ECS_C uint32_t TransformManager_GetParent_C(ECS::TransformManager_Interface*tm, uint32_t entity);
//DECLDIR_ECS_C uint32_t TransformManager_GetNumberOfChildren_C(ECS::TransformManager_Interface*tm, uint32_t entity);
//DECLDIR_ECS_C void TransformManager_GetChildren_C(ECS::TransformManager_Interface*tm, uint32_t entity, uint32_t* children);
//DECLDIR_ECS_C void TransformManager_SetPosition_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector position);
//DECLDIR_ECS_C ECS::Vector TransformManager_GetPosition_C(ECS::TransformManager_Interface*tm, uint32_t entity);
//DECLDIR_ECS_C void TransformManager_SetRotation_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector rotation);
//DECLDIR_ECS_C ECS::Vector TransformManager_GetRotation_C(ECS::TransformManager_Interface*tm, uint32_t entity);
//DECLDIR_ECS_C void TransformManager_SetScale_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector scale);
//DECLDIR_ECS_C ECS::Vector TransformManager_GetScale_C(ECS::TransformManager_Interface*tm, uint32_t entity);
//DECLDIR_ECS_C void TransformManager_SetTransform_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Matrix transform);
//DECLDIR_ECS_C ECS::Matrix TransformManager_GetTransform_C(ECS::TransformManager_Interface*tm, uint32_t entity);
//DECLDIR_ECS_C void TransformManager_RegisterTransformUser(ECS::TransformManager_Interface* tm, ECS::Manager_TransformUser* tu);
//DECLDIR_ECS_C void TransformManager_UnregisterTransformUser(ECS::TransformManager_Interface* tm, ECS::Manager_TransformUser* tu);
//DECLDIR_ECS_C void TransformManager_GetPosition_NoReturn_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector* position);
//DECLDIR_ECS_C void TransformManager_GetRotation_NoReturn_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector* rotation);
//DECLDIR_ECS_C void TransformManager_GetScale_NoReturn_C(ECS::TransformManager_Interface*tm, uint32_t entity, ECS::Vector* scale);

#endif