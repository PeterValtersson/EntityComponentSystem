#ifndef _DATA_MANAGER_INTERFACE_H_
#define _DATA_MANAGER_INTERFACE_H_
#pragma once
#include "Manager_Base.h"
#include <variant>
#include <string>

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
		
		virtual void create( Entity entity )noexcept = 0;
		virtual void create_multiple( Entity entity )noexcept = 0;

		virtual void set_data( Entity entity, Utilities::GUID key, Data data )noexcept = 0;
		virtual void remove_data( Entity enitity, Utilities::GUID key )noexcept = 0;
		virtual Data get_data( Entity entity, Utilities::GUID key )const noexcept = 0;
		template <typename T>
		inline T get_data( Entity entity, Utilities::GUID key )const
		{
			return get_data<T>( entity, key );
		}
	};
}

#endif