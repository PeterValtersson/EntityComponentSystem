#include "RenderObjectManager.h"
#include <Utilities/Profiler/Profiler.h>

template<class T, size_t N>
void mem_set( T( &dst )[N], const T& src )
{
	for ( size_t i = 0; i < N; i++ )
		dst[i] = src;
}

ECS::RenderObjectManager::RenderObjectManager( const RenderObjectManager_InitializationInfo& ii ) : instancing( ii.renderer )
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
	entries.get<Entries::Pipeline>( index ).Edit( []( Renderer::Pipeline::Pipeline_Mutable p )
	{
		p.OMStage.renderTargetCount = 1;
		p.OMStage.renderTargets[0] = Renderer::Default_RenderTarget;
		p.RStage.viewport = Renderer::Default_Viewport;
	} );
}

void ECS::RenderObjectManager::Edit_Pipeline( Entity entity, const std::function<void( Renderer::Pipeline::Pipeline_Mutable )>& callback ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto pipeline_copy = entries.peek<Entries::Pipeline>( *find );
		entries.get<Entries::Pipeline>( *find ).Edit( callback );
	}
}


void ECS::RenderObjectManager::ToggleVisible( const Entity entity, bool visible ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto& v = entries.get<Entries::Visible>( *find );
		if ( v == visible )
			return;
		v = visible;

		if ( visible )
		{
			Renderer::RenderJob job;
			job.pipeline = entries.peek<Entries::Pipeline>( *find );
			ResourceHandler::Resource mesh( job.pipeline.IAStage().vertexBuffer );
			job.vertexCount = mesh.get_copy<MeshInfo>().vertex_count[0];
			job.maxInstances = 256;
			instancing.AddEntity( entity, job );
		}
		else 
		{
			instancing.RemoveEntity( entity );
		}
		
	}
}

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
{
	entries.erase( entity );
}

void ECS::RenderObjectManager::DestroyMultiple( const Entity entities[], size_t numEntities ) noexcept
{
	for ( size_t i = 0; i < numEntities; i++ )
		entries.erase( entities[i] );
}

void ECS::RenderObjectManager::DestroyMultiple( const std::vector<Entity>& entities ) noexcept
{
	for ( auto e : entities )
		entries.erase( e );
}

void ECS::RenderObjectManager::DestroyAll() noexcept
{
	entries.clear();
}

size_t ECS::RenderObjectManager::GetNumberOfRegisteredEntities() const noexcept
{
	return entries.size();
}

void ECS::RenderObjectManager::GetRegisteredEntities( Entity entities[], size_t numEntities ) const noexcept
{
	memcpy( entities, entries.peek<Entries::Entity>(), std::min( numEntities, entries.size() ) );
}

std::vector<ECS::Entity> ECS::RenderObjectManager::GetRegisteredEntities() const noexcept
{
	std::vector<ECS::Entity> v( entries.size());
	GetRegisteredEntities( v.data(), entries.size() );
	return v;
}

void ECS::RenderObjectManager::Frame() noexcept
{
	GarbageCollection();
}

Utilities::GUID ECS::RenderObjectManager::GetManagerType() const noexcept
{
	return "RenderObjectManager";
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
{
	PROFILE;
	size_t alive_in_row = 0;
	const size_t quitWhenReached = std::max( entries.size() / 10, 40Ui64 );
	while ( entries.size() > 0 && alive_in_row < quitWhenReached )
	{
		std::uniform_int_distribution<size_t> distribution( 0U, entries.size() - 1U );
		size_t i = distribution( generator );
		if ( initInfo.entityManager->IsAlive( entries.get<Entries::Entity>( i ) ) )
		{
			alive_in_row++;
			continue;
		}
		alive_in_row = 0;
		entries.erase( i );
	}
}
