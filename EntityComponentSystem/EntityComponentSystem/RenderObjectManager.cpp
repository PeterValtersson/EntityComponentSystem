#include "RenderObjectManager.h"
#include <Utilities/Profiler/Profiler.h>

template<class T, size_t N>
void mem_set( T( &dst )[N], const T& src )
{
	for ( size_t i = 0; i < N; i++ )
		dst[i] = src;
}

ECS::RenderObjectManager::RenderObjectManager( const RenderObjectManager_InitializationInfo& ii )
	: instancing( ii.renderer ),
	default_mesh("default_mesh", ResourceHandler::Flags::Persistent),
	default_shader("default_vertex_shader", ResourceHandler::Flags::Persistent )
{
	default_mesh.check_in();
	default_shader.check_in();
}

ECS::RenderObjectManager::~RenderObjectManager()
{}

void ECS::RenderObjectManager::Create( Entity entity ) noexcept
{
	PROFILE;
	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;

	if ( auto find = entries.find( entity ); find.has_value() )
		return;

	auto index = entries.add( entity );
	entries.get<Entries::Mesh>( index ) = default_mesh;
	entries.get<Entries::Shader>( index ) = default_shader;
}

void ECS::RenderObjectManager::Set_Shader( Entity entity, Utilities::GUID shader ) noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		entries.get<Entries::Shader>( *find ) = shader;
	}
}

void ECS::RenderObjectManager::Set_Mesh( Entity entity, Utilities::GUID mesh ) noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		entries.get<Entries::Mesh>( *find ) = mesh;
	}
}

Utilities::GUID ECS::RenderObjectManager::Get_Shader( Entity entity ) noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); find.has_value() )
		return entries.peek<Entries::Shader>( *find );
	else
		return Utilities::GUID::invalid;
}

Utilities::GUID ECS::RenderObjectManager::Get_Mesh( Entity entity ) noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); find.has_value() )
		return entries.peek<Entries::Mesh>( *find );
	else
		return Utilities::GUID::invalid;
}


void ECS::RenderObjectManager::ToggleVisible( const Entity entity, bool visible ) noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto& v = entries.get<Entries::Visible>( *find );
		if ( v == visible )
			return;
		v = visible;

		if ( visible )
		{
			Renderer::RenderJob job;
			job.pipeline = Renderer::Pipeline::Pipeline();

			job.pipeline.Edit( [&]( Renderer::Pipeline::Pipeline_Mutable p )
			{
				p.OMStage.renderTargetCount = 1;
				p.OMStage.renderTargets[0] = Renderer::Default_RenderTarget;
				p.RStage.viewport = Renderer::Default_Viewport;
				if ( flag_has( entries.peek<Entries::Mesh>( *find ).get_status(), ResourceHandler::Status::In_Memory_Parsed ) )
				{
					p.IAStage.vertexBuffer = entries.peek<Entries::Mesh>( *find );
					job.vertexCount = entries.peek<Entries::Mesh>( *find ).get_copy<MeshInfo>().vertex_count[0];
				}
				else
				{
					p.IAStage.vertexBuffer = default_mesh;
					job.vertexCount = default_mesh.get_copy<MeshInfo>().vertex_count[0];
					add_entity_to_update_mesh( entity );
				}
				if ( flag_has( entries.peek<Entries::Shader>( *find ).get_status(), ResourceHandler::Status::In_Memory_Parsed ) )
					p.VSStage.shader = entries.peek<Entries::Shader>( *find );
				else
				{
					p.VSStage.shader = default_shader;
					add_entity_to_update_shader( entity );
				}
			} );

			job.maxInstances = 256;
			instancing.AddEntity( entity, job );
		}
		else 
		{
			instancing.RemoveEntity( entity );
			remove_entity_to_update_mesh( entity );
			remove_entity_to_update_shader( entity );
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
	memcpy( entities, entries.peek<Entries::Entity>().data(), std::min( numEntities, entries.size() ) );
}

std::vector<ECS::Entity> ECS::RenderObjectManager::GetRegisteredEntities() const noexcept
{
	return entries.peek<Entries::Entity>();
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

void ECS::RenderObjectManager::update_mesh_shaders()
{
	for ( size_t i = 0; i < entities_to_change_mesh.size(); i++)
	{
		auto ent = entities_to_change_mesh[i];
		if ( auto find = entries.find( ent ); find.has_value() )
		{
			if ( flag_has( entries.peek<Entries::Mesh>( *find ).get_status(), ResourceHandler::Status::In_Memory_Parsed ) )
			{
				Renderer::RenderJob job;
				job.pipeline = Renderer::Pipeline::Pipeline();

				job.pipeline.Edit( [&]( Renderer::Pipeline::Pipeline_Mutable p )
				{
					p.OMStage.renderTargetCount = 1;
					p.OMStage.renderTargets[0] = Renderer::Default_RenderTarget;
					p.RStage.viewport = Renderer::Default_Viewport;

					p.IAStage.vertexBuffer = entries.peek<Entries::Mesh>( *find );
					job.vertexCount = entries.peek<Entries::Mesh>( *find ).get_copy<MeshInfo>().vertex_count[0];
					if ( flag_has( entries.peek<Entries::Shader>( *find ).get_status(), ResourceHandler::Status::In_Memory_Parsed ) )
						p.VSStage.shader = entries.peek<Entries::Shader>( *find );
					else
					{
						p.VSStage.shader = default_shader;
					}
				} );

				job.maxInstances = 256;
				instancing.AddEntity( ent, job );
				entities_to_change_mesh[i] = entities_to_change_mesh.back();
				entities_to_change_mesh.pop_back();
				i--;
			}
		}
		else
		{
			entities_to_change_mesh[i] = entities_to_change_mesh.back();
			entities_to_change_mesh.pop_back();
			i--;
		}
	}

	for ( size_t i = 0; i < entities_to_change_shader.size(); i++ )
	{
		auto ent = entities_to_change_shader[i];
		if ( auto find = entries.find( ent ); find.has_value() )
		{
			if ( flag_has( entries.peek<Entries::Shader>( *find ).get_status(), ResourceHandler::Status::In_Memory_Parsed ) )
			{
				Renderer::RenderJob job;
				job.pipeline = Renderer::Pipeline::Pipeline();

				job.pipeline.Edit( [&]( Renderer::Pipeline::Pipeline_Mutable p )
				{
					p.OMStage.renderTargetCount = 1;
					p.OMStage.renderTargets[0] = Renderer::Default_RenderTarget;
					p.RStage.viewport = Renderer::Default_Viewport;

					if ( flag_has( entries.peek<Entries::Mesh>( *find ).get_status(), ResourceHandler::Status::In_Memory_Parsed ) )
					{
						p.IAStage.vertexBuffer = entries.peek<Entries::Mesh>( *find );
						job.vertexCount = entries.peek<Entries::Mesh>( *find ).get_copy<MeshInfo>().vertex_count[0];
					}
					else
					{
						p.IAStage.vertexBuffer = default_mesh;
						job.vertexCount = default_mesh.get_copy<MeshInfo>().vertex_count[0];
					}
					p.VSStage.shader = entries.peek<Entries::Shader>( *find );
				} );

				job.maxInstances = 256;
				instancing.AddEntity( ent, job );
				entities_to_change_shader[i] = entities_to_change_shader.back();
				entities_to_change_shader.pop_back();
				i--;
			}
		}
		else
		{
			entities_to_change_shader[i] = entities_to_change_shader.back();
			entities_to_change_shader.pop_back();
			i--;
		}
	}
}

void ECS::RenderObjectManager::add_entity_to_update_mesh( Entity entity )
{
	if ( std::find( entities_to_change_mesh.begin(), entities_to_change_mesh.end(), entity ) != entities_to_change_mesh.end() )
		entities_to_change_mesh.push_back( entity );
}

void ECS::RenderObjectManager::remove_entity_to_update_mesh( Entity entity )
{
	for ( size_t i = 0; i < entities_to_change_mesh.size(); i++ )
	{
		if ( entities_to_change_mesh[i] = entity )
		{
			entities_to_change_mesh[i] = entities_to_change_mesh.back();
			entities_to_change_mesh.pop_back();
		}
	}
}

void ECS::RenderObjectManager::add_entity_to_update_shader( Entity entity )
{
	if ( std::find( entities_to_change_shader.begin(), entities_to_change_shader.end(), entity ) != entities_to_change_shader.end() )
		entities_to_change_shader.push_back( entity );
}

void ECS::RenderObjectManager::remove_entity_to_update_shader( Entity entity )
{
	for ( size_t i = 0; i < entities_to_change_shader.size(); i++ )
	{
		if ( entities_to_change_shader[i] = entity )
		{
			entities_to_change_shader[i] = entities_to_change_shader.back();
			entities_to_change_shader.pop_back();
		}
	}
}
