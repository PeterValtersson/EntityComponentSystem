#include <Managers/CameraManager_Interface.h>
#include "CameraManager.h"

DECLDIR_ECS std::shared_ptr<ECS::CameraManager_Interface> ECS::CameraManager_Interface::create_manager( const CameraManager_Init_Info& init_info )
{
	auto m = std::make_shared<CameraManager>( init_info );
	init_info.entity_manager->RegisterManagerForDestroyNow( m );
	return m;
}
