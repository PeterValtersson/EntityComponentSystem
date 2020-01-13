#include <Managers/PropertyManager_Interface.h>
#include "PropertyManager.h"

DECLDIR_ECS std::shared_ptr<ECS::PropertyManager_Interface> ECS::PropertyManager_Interface::create_manager( PropertyManager_Init_Info init_info )
{
	auto m = std::make_shared<PropertyManager>( init_info );
	init_info.entity_manager->RegisterManagerForDestroyNow( m );
	return m;
}
