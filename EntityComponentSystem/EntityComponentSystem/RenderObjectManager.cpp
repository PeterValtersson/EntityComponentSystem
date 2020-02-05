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

void ECS::RenderObjectManager::Create( Entity entity, ResourceHandler::Resource mesh, ResourceHandler::Resource shader, RenderableFlags render_flags, MeshFlags mesh_flags ) noexcept
{
	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;
	if ( auto find = entries.find( entity ); find.has_value() )
		return;
	auto index = entries.add( entity );
	entries.get<Entries::Mesh>( index ) = mesh;
	mem_set( entries.get<Entries::Shader>( index ), shader );
	entries.get<Entries::MeshInfo>( index ).sub_mesh_count = 0;
}
void ECS::RenderObjectManager::SetMesh( Entity entity, ResourceHandler::Resource mesh ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		entries.get<Entries::Mesh>( *find ) = mesh;
	}
}
void ECS::RenderObjectManager::SetShader( Entity entity, ResourceHandler::Resource shader ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		mem_set( entries.get<Entries::Shader>( *find ), shader );
	}
}
void ECS::RenderObjectManager::SetShader( Entity entity, uint8_t subMesh, ResourceHandler::Resource shader ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		if ( subMesh < MeshInfo::max_sub_mesh_count )
			entries.get<Entries::Shader>( *find )[subMesh] = shader;
	}
}
void ECS::RenderObjectManager::ToggleWireframe( const Entity entity, bool wireFrame ) noexcept
{}

void ECS::RenderObjectManager::ToggleWireframe( const Entity entity, uint8_t submesh, bool wireFrame ) noexcept
{}

void ECS::RenderObjectManager::ToggleTransparency( const Entity entity, bool transparent ) noexcept
{}

void ECS::RenderObjectManager::ToggleTransparency( const Entity entity, uint8_t submesh, bool transparent ) noexcept
{}

void ECS::RenderObjectManager::ToggleShadow( const Entity entity, bool cast_shadow ) noexcept
{}

void ECS::RenderObjectManager::ToggleShadow( const Entity entity, uint8_t submesh, bool cast_shadow ) noexcept
{}

void ECS::RenderObjectManager::ToggleVisible( const Entity entity, bool visible ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		// Check if everything is already visible/invisible
		auto& sub_visible = entries.get<Entries::Visible>( *find );
		{

			bool same = true;
			for ( uint8_t i = 0; i < MeshInfo::max_sub_mesh_count; i++ )
				if ( sub_visible[i] != visible )
				{
					same = false;
					break;
				}
			if ( same )
				return; // Nothing to do, so return.
		}


		Renderer::RenderJob job;






		for ( uint8_t i = 0; i < MeshInfo::max_sub_mesh_count; i++ )
			sub_visible[i] = visible;


	}
}

void ECS::RenderObjectManager::ToggleVisible( const Entity entity, uint8_t submesh, bool visible ) noexcept
{}

std::vector<std::string> ECS::RenderObjectManager::GetSubmeshes( const Entity entity ) noexcept
{
	std::vector<std::string> sub_meshes;
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		Force_Load_Mesh( *find );
		auto& mi = entries.peek<Entries::MeshInfo>( *find );
		for ( size_t i = 0; i < mi.sub_mesh_count; i++ )
			sub_meshes.push_back( mi.name[i] );
	}
	return sub_meshes;
}

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

void ECS::RenderObjectManager::Force_Load_Mesh( size_t i ) noexcept
{
	entries.get<Entries::MeshInfo>( i ) = entries.get<Entries::Mesh>( i ).get_copy<MeshInfo>();
}
