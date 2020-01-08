#include <Managers/DataManager_Interface.h>
#include "DataManager.h"

DECLDIR_ECS std::shared_ptr<ECS::DataManager_Interface> ECS::DataManager_Interface::create_manager( DataManager_Init_Info init_info )
{
	return std::make_shared<DataManager>( init_info );
}
