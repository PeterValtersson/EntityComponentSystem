#ifndef _DATA_MANAGER_INTERFACE_H_
#define _DATA_MANAGER_INTERFACE_H_
#pragma once
#include "Manager_Base.h"
#include <variant>
#include <string>
#include <vector>

namespace ECS
{
	typedef std::variant<uint64_t, uint32_t, int, double, float, std::string> EntityProperty;
	struct PropertyManager_Init_Info {
		std::shared_ptr<EntityManager_Interface> entity_manager;
		void* pNext = nullptr;
	};
	class PropertyManager_Interface : public Manager_Base {
	public:
		DECLDIR_ECS static std::shared_ptr<PropertyManager_Interface> create_manager( PropertyManager_Init_Info init_info);
		
		virtual void Create( Entity entity )noexcept = 0;
		virtual void CreateMultiple( const std::vector<Entity>& entities )noexcept = 0;

		virtual void CreateProperty( Entity entity, std::string_view key, EntityProperty data )noexcept = 0;
		virtual void SetProperty( Entity entity, Utilities::GUID key, EntityProperty data )noexcept = 0;
		virtual void RemoveProperty( Entity entity, Utilities::GUID key )noexcept = 0;

		virtual EntityProperty GetProperty( Entity entity, Utilities::GUID key )const noexcept = 0;
		virtual std::string GetPropertyKey( Entity entity, Utilities::GUID key )const noexcept = 0;
		virtual size_t GetNumProperties( Entity entity )const noexcept = 0;

		template <typename T>
		inline T GetProperty( Entity entity, Utilities::GUID key )const
		{
			return std::get<T>(GetProperty( entity, key ));
		}
	};
}

#endif