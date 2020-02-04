#include "MeshManager.h"

ECS::MeshManager::MeshManager( const MeshManager_InitializationInfo& ii )
{}

ECS::MeshManager::~MeshManager()
{}

void ECS::MeshManager::Create( Entity entity, ResourceHandler::Resource mesh, ResourceHandler::Resource shader, RenderableFlags render_flags, MeshFlags mesh_flags ) noexcept
{
	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;
	if ( auto find = entries.find( entity ); find.has_value() )
		return;
}

void ECS::MeshManager::SetDefaultMesh( Utilities::GUID mesh )
{}

Utilities::GUID ECS::MeshManager::GetDefaultMesh() const noexcept
{
	return Utilities::GUID();
}

void ECS::MeshManager::SetDefaultShader( Utilities::GUID shader )
{}

Utilities::GUID ECS::MeshManager::GetDefaultShader() const noexcept
{
	return Utilities::GUID();
}

void ECS::MeshManager::ToggleWireframe( const Entity entity, bool wireFrame ) noexcept
{}

void ECS::MeshManager::ToggleWireframe( const Entity entity, uint8_t submesh, bool wireFrame ) noexcept
{}

void ECS::MeshManager::ToggleTransparency( const Entity entity, bool transparent ) noexcept
{}

void ECS::MeshManager::ToggleTransparency( const Entity entity, uint8_t submesh, bool transparent ) noexcept
{}

void ECS::MeshManager::ToggleShadow( const Entity entity, bool cast_shadow ) noexcept
{}

void ECS::MeshManager::ToggleShadow( const Entity entity, uint8_t submesh, bool cast_shadow ) noexcept
{}

void ECS::MeshManager::ToggleVisible( const Entity entity, bool visible ) noexcept
{}

void ECS::MeshManager::ToggleVisible( const Entity entity, uint8_t submesh, bool visible ) noexcept
{}

std::vector<std::string> ECS::MeshManager::GetSubmeshes( const Entity entity ) const noexcept
{
	return std::vector<std::string>();
}

bool ECS::MeshManager::is_registered( Entity entity ) const noexcept
{
	return false;
}

void ECS::MeshManager::CreateFromResource( Entity entity, ResourceHandler::Resource resource )
{}

uint64_t ECS::MeshManager::GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )> & writer ) const noexcept
{
	return uint64_t();
}

void ECS::MeshManager::Destroy( Entity entity ) noexcept
{}

void ECS::MeshManager::DestroyMultiple( const Entity entities[], size_t numEntities ) noexcept
{}

void ECS::MeshManager::DestroyMultiple( const std::vector<Entity> & entities ) noexcept
{}

void ECS::MeshManager::DestroyAll() noexcept
{}

size_t ECS::MeshManager::GetNumberOfRegisteredEntities() const noexcept
{
	return size_t();
}

void ECS::MeshManager::GetRegisteredEntities( Entity entities[], size_t numEntities ) const noexcept
{}

std::vector<ECS::Entity> ECS::MeshManager::GetRegisteredEntities() const noexcept
{
	return std::vector<Entity>();
}

void ECS::MeshManager::Frame() noexcept
{}

Utilities::GUID ECS::MeshManager::GetManagerType() const noexcept
{
	return Utilities::GUID();
}

size_t ECS::MeshManager::get_memory_usage() const noexcept
{
	return size_t();
}

void ECS::MeshManager::shrink_to_fit()
{}

void ECS::MeshManager::write_to_stream( std::ostream & stream ) const
{}

void ECS::MeshManager::read_from_stream( std::istream & stream )
{}

void ECS::MeshManager::GarbageCollection() noexcept
{}
