#include "PropertyManager.h"

ECS::PropertyManager::PropertyManager( PropertyManager_Init_Info init_info )
{
	if ( !init_info.entity_manager )
		throw CouldNotCreateManager( "Property Manager must have an entity manager" );
}

void ECS::PropertyManager::Create( Entity entity ) noexcept
{
	if ( auto find = entries.find( entity ); !find.has_value() )
		entries.add( entity );
}

void ECS::PropertyManager::CreateMultiple( const std::vector<Entity>& entities ) noexcept
{
	for ( auto e : entities )
		if ( auto find = entries.find( e ); !find.has_value() )
			entries.add( e );
}

void ECS::PropertyManager::CreateProperty( Entity entity, std::string_view key, EntityProperty data ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto& properties = entries.get<Entries::Properties>( *find );
		if ( auto findp = properties.find( key ); findp.has_value() )
			properties.set<EntityProperties::Property>( *find, data );
		else
			properties.add( key, std::string( key ), data );
	}

}

void ECS::PropertyManager::SetProperty( Entity entity, Utilities::GUID key, EntityProperty data ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto& properties = entries.get<Entries::Properties>( *find );
		if ( auto findp = properties.find( key ); findp.has_value() )
			properties.set<EntityProperties::Property>( *findp, data );
	}
}

void ECS::PropertyManager::RemoveProperty( Entity entity, Utilities::GUID key ) noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
		entries.get<Entries::Properties>( *find ).erase( key );
}

ECS::EntityProperty ECS::PropertyManager::GetProperty( Entity entity, Utilities::GUID key ) const noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto& properties = entries.peek<Entries::Properties>( *find );
		if ( auto findp = properties.find( key ); findp.has_value() )
			return properties.peek<EntityProperties::Property>( *findp );
	}
	return {};
}

std::string ECS::PropertyManager::GetPropertyKey( Entity entity, Utilities::GUID key ) const noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
	{
		auto& properties = entries.peek<Entries::Properties>( *find );
		if ( auto findp = properties.find( key ); findp.has_value() )
			return properties.peek<EntityProperties::KeyString>( *findp );
	}
	return "";
}

size_t ECS::PropertyManager::GetNumProperties( Entity entity ) const noexcept
{
	if ( auto find = entries.find( entity ); find.has_value() )
		return entries.peek<Entries::Properties>( *find ).size();
	return 0;
}

bool ECS::PropertyManager::is_registered( Entity entity ) const noexcept
{
	return entries.find( entity ).has_value();
}

void ECS::PropertyManager::CreateFromResource( Entity entity, Resources::Resource resource )
{}

uint64_t ECS::PropertyManager::GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer ) const noexcept
{
	return uint64_t();
}

void ECS::PropertyManager::Destroy( Entity entity ) noexcept
{
	entries.erase( entity );
}

void ECS::PropertyManager::DestroyMultiple( const Entity entities[], size_t numEntities ) noexcept
{
	for ( size_t i = 0; i < numEntities; i++ )
		entries.erase( entities[i] );
}

void ECS::PropertyManager::DestroyMultiple( const std::vector<Entity>& entities ) noexcept
{
	for ( auto e : entities )
		entries.erase( e );
}

void ECS::PropertyManager::DestroyAll() noexcept
{
	entries.clear();
}

void ECS::PropertyManager::ToggleActive( Entity entity, bool active ) noexcept
{}

void ECS::PropertyManager::ToggleActive( const Entity entities[], size_t numEntities, bool active ) noexcept
{}

void ECS::PropertyManager::ToggleActive( const std::vector<Entity>& entities, bool active ) noexcept
{}

size_t ECS::PropertyManager::GetNumberOfRegisteredEntities() const noexcept
{
	return entries.size();
}

void ECS::PropertyManager::GetRegisteredEntities( Entity entities[], size_t numEntities ) const noexcept
{
	auto& ents = entries.peek<Entries::Entity>();
	if ( entries.size() == numEntities )
		memcpy( entities, ents.data(), sizeof( Entity ) * numEntities );
}

std::vector<ECS::Entity> ECS::PropertyManager::GetRegisteredEntities() const noexcept
{
	return entries.peek<Entries::Entity>();
}

void ECS::PropertyManager::Frame() noexcept
{}

Utilities::GUID ECS::PropertyManager::GetManagerType() const noexcept
{
	return "PropertyManager"_hash;
}

size_t ECS::PropertyManager::get_memory_usage() const noexcept
{
	return size_t();
}

void ECS::PropertyManager::shrink_to_fit()
{}

void ECS::PropertyManager::write_to_stream( std::ostream& stream ) const
{}

void ECS::PropertyManager::read_from_stream( std::istream& stream )
{}

void ECS::PropertyManager::GarbageCollection() noexcept
{}
