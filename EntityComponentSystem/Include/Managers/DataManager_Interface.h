#ifndef _DATA_MANAGER_INTERFACE_H_
#define _DATA_MANAGER_INTERFACE_H_
#pragma once
#include "Manager_Base.h"
#include <variant>
#include <string>
#include <vector>

namespace ECS
{
	typedef std::variant<int, float, std::string> Data;
	struct DataManager_Init_Info {
		std::shared_ptr<EntityManager_Interface> entity_manager;
		void* pNext = nullptr;
	};
	class DataManager_Interface : public Manager_Base {
	public:
		DECLDIR_ECS static std::shared_ptr<DataManager_Interface> create_manager( DataManager_Init_Info init_info);
		
		virtual void Create( Entity entity )noexcept = 0;
		virtual void CreateMultiple( const std::vector<Entity>& entities )noexcept = 0;

		virtual void CreateData( Entity entity, std::string_view key, Data data )noexcept = 0;
		virtual void SetData( Entity entity, Utilities::GUID key, Data data )noexcept = 0;
		virtual void RemoveData( Entity enitity, Utilities::GUID key )noexcept = 0;
		virtual Data GetData( Entity entity, Utilities::GUID key )const noexcept = 0;
		template <typename T>
		inline T GetData( Entity entity, Utilities::GUID key )const
		{
			return GetData<T>( entity, key );
		}
	};
}

#endif