#include "RenderObjectManager.h"

template<class T, size_t N>
void mem_set( T( &dst )[N], const T& src )
{
	for ( size_t i = 0; i < N; i++ )
		dst[i] = src;
}

ECS::RenderObjectManager::RenderObjectManager( const RenderObjectManager_InitializationInfo& ii )
{

}

ECS::RenderObjectManager::~RenderObjectManager()
{}

void ECS::RenderObjectManager::Create( Entity entity ) noexcept
{
	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;

	if ( auto find = entries.find( entity ); find.has_value() )
		return;

	auto index = entries.add( entity );
	entries.get<Entries::Pipeline>( index ) = Renderer::Pipeline::Pipeline();
}

void ECS::RenderObjectManager::Edit_Pipeline( Entity entity, const std::function<void( Renderer::Pipeline::Pipeline_Mutable )> & callback ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto pipeline_copy = entries.peek<Entries::Pipeline>( *find );
		entries.get<Entries::Pipeline>( *find ).Edit( callback );
	}
}


void ECS::RenderObjectManager::ToggleVisible( const Entity entity, bool visible ) noexcept
{}

void ECS::RenderObjectManager::ToggleVisible( const Entity entity, uint8_t submesh, bool visible ) noexcept
{}


bool ECS::RenderObjectManager::is_registered( Entity entity ) const noexcept
{
	return entries.find( entity ).has_value();
}

void ECS::RenderObjectManager::CreateFromResource( Entity entity, ResourceHandler::Resource resource )
{}

uint64_t ECS::RenderObjectManager::GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer ) const noexcept
{
	return uint64_t();
}

void ECS::RenderObjectManager::Destroy( Entity entity ) noexcept
{}

void ECS::RenderObjectManager::DestroyMultiple( const Entity entities[], size_t numEntities ) noexcept
{}

void ECS::RenderObjectManager::DestroyMultiple( const std::vector<Entity>& entities ) noexcept
{}

void ECS::RenderObjectManager::DestroyAll() noexcept
{}

size_t ECS::RenderObjectManager::GetNumberOfRegisteredEntities() const noexcept
{
	return size_t();
}

void ECS::RenderObjectManager::GetRegisteredEntities( Entity entities[], size_t numEntities ) const noexcept
{}

std::vector<ECS::Entity> ECS::RenderObjectManager::GetRegisteredEntities() const noexcept
{
	return std::vector<Entity>();
}

void ECS::RenderObjectManager::Frame() noexcept
{}

Utilities::GUID ECS::RenderObjectManager::GetManagerType() const noexcept
{
	return Utilities::GUID();
}

size_t ECS::RenderObjectManager::get_memory_usage() const noexcept
{
	return size_t();
}

void ECS::RenderObjectManager::shrink_to_fit()
{}

void ECS::RenderObjectManager::write_to_stream( std::ostream& stream ) const
{}

void ECS::RenderObjectManager::read_from_stream( std::istream& stream )
{}

void ECS::RenderObjectManager::GarbageCollection() noexcept
{}
