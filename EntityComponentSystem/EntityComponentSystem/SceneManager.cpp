#include "SceneManager.h"
#include <Utilities/Profiler/Profiler.h>
#include <Utilities/FStreamHelpers.h>

ECS::SceneManager::SceneManager( SceneManager_Init_Info ii ) : initInfo( ii )
{
	_ASSERT_EXPR( initInfo.entityManager, L"SceneManager must have entitymanager" );
	_ASSERT_EXPR( initInfo.transformManager, L"SceneManager must have transform manager" );

}

ECS::SceneManager::~SceneManager()
{}

uint32_t ECS::SceneManager::GetNumberOfChildResourcesOfSceneResource( Resources::Resource resource ) const
{
	PROFILE;
	uint32_t numCS = 0;
	resource.use_data( [&]( const Utilities::Memory::ConstMemoryBlock data )
	{
		auto s = Utilities::Binary_Stream::create_stream_from_data( data.get_char(), data.used_size );

		decltype( version ) ver = 0;
		Utilities::Binary_Stream::read( s.stream, ver );
		std::string name;
		Utilities::Binary_Stream::read_string_w_size( s.stream, name );


		s.stream.read( ( char* )&numCS, sizeof( numCS ) );
	} );

	return numCS;
}

void ECS::SceneManager::GetChildResourcesOfSceneResource( Resources::Resource resource, Utilities::GUID resources[], uint32_t num ) const
{
	PROFILE;
	resource.use_data( [&]( const Utilities::Memory::ConstMemoryBlock data )
	{
		auto s = Utilities::Binary_Stream::create_stream_from_data( data.get_char(), data.used_size );

		decltype( version ) ver = 0;
		Utilities::Binary_Stream::read( s.stream, ver );
		std::string name;
		Utilities::Binary_Stream::read_string_w_size( s.stream, name );

		uint32_t numCS = 0;
		Utilities::Binary_Stream::read( s.stream, numCS );
		numCS = std::min( numCS, num );
		Utilities::Binary_Stream::read( s.stream, resources, numCS * sizeof( Utilities::GUID ) );
	} );
}

void ECS::SceneManager::RemoveEntityFromScene( Entity scene, Entity entity )noexcept
{
	PROFILE;
	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
	{
		auto& map = entries.get<Entries::EntityToEntityInScene>( *find );
		if ( auto findE = map.find( entity ); findE == map.end() )
			return;
		else
		{
			auto& entities = entries.get<Entries::EntitiesInScene>( *find );
			auto& names = entries.get<Entries::EntitiesInSceneNames>( *find );
			uint32_t last = static_cast< uint32_t >( entities.size() - 1 );
			auto lastE = entities[last];

			entities[findE->second] = entities[last];
			names[findE->second] = names[last];

			entities.pop_back();
			names.pop_back();

			map[lastE] = findE->second;
			map.erase( entity );
		}
	}
}

void ECS::SceneManager::SetNameOfEntityInScene( Entity scene, Entity entity, const std::string& name )noexcept
{
	PROFILE;
	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
	{
		auto& map = entries.get<Entries::EntityToEntityInScene>( *find );
		if ( auto findE = map.find( entity ); findE == map.end() )
			return;
		else
		{
			entries.get<Entries::EntitiesInSceneNames>( *find )[findE->second] = name;
		}
	}
}

std::string ECS::SceneManager::GetNameOfEntityInScene( Entity scene, Entity entity ) const noexcept
{
	PROFILE;
	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
	{
		auto& map = entries.peek<Entries::EntityToEntityInScene>( *find );
		if ( auto findE = map.find( entity ); findE == map.end() )
			return "";
		else
			return entries.peek<Entries::EntitiesInSceneNames>( *find )[findE->second];
	}
}

std::string ECS::SceneManager::GetNameOfScene( Entity scene ) const noexcept
{
	PROFILE;
	if ( auto find = entries.find( scene ); !find.has_value() )
		return "";
	else
		return entries.peek<Entries::Name>( *find );
}

void ECS::SceneManager::SetNameOfScene( Entity scene, const std::string& name )noexcept
{
	PROFILE;
	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
		entries.get<Entries::Name>( *find ) = name;
}

void ECS::SceneManager::RegisterManager( Manager_Base* manager )noexcept
{
	PROFILE;
	for ( auto& m : managers )
		if ( m == manager )
			return;
	managers.push_back( manager );
}


void ReadComponents( ECS::SceneManager* sm, const std::string& name, ECS::Entity entity, std::istream& stream, uint32_t componentCount, const std::vector<ECS::Manager_Base*>& managers )
{
	PROFILE;
	/*for ( uint32_t i = 0; i < componentCount; i++ )
	{
		Utilities::GUID type;
		Utilities::Binary_Stream::read( stream, type );
		if ( sm && type == sm->GetManagerType() )
			sm->CreateFromResource( entity, Resources::Resource( name, sm->GetManagerType() ) );
		else
			for ( auto& m : managers )
				if ( m->GetManagerType() == type )
					m->CreateFromStream( entity, stream );

	}*/
};

void ECS::SceneManager::CreateFromResource( Entity entity, Resources::Resource resource )
{
	PROFILE;
	/*if ( auto find = entries.find( entity ); !find.has_value() )
		return;

	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;

	resource.use_data( [&, this]( const Utilities::Memory::ConstMemoryBlock data )
	{
		auto s = Utilities::Binary_Stream::create_stream_from_data( data.get_char(), data.used_size );

		decltype( version ) ver = 0;
		Utilities::Binary_Stream::read( s.stream, ver );
		std::string name;
		Utilities::Binary_Stream::read_string_w_size( s.stream, name );

		auto index = entries.add( entity );
		entries.set<Entries::Name>( index, name );

		uint32_t numCS = 0;
		Utilities::Binary_Stream::read( s.stream, numCS );
		std::vector<Utilities::GUID> resources( numCS );
		Utilities::Binary_Stream::read( s.stream, resources.data(), numCS * sizeof( Utilities::GUID ) );

		uint32_t componentCount = 0;
		Utilities::Binary_Stream::read( s.stream, componentCount );
		ReadComponents( nullptr, name, entity, s.stream, componentCount, managers );

		uint32_t numEnts = 0;
		s.stream.read( ( char* )&numEnts, sizeof( numEnts ) );
		entries.entitiesInScene[index].resize( numEnts );
		entries.entityNamesInScene[index].resize( numEnts );
		initInfo.entityManager->CreateMultiple( entries.entitiesInScene[index].data(), uint32_t( entries.entitiesInScene[index].size() ) );
		for ( uint32_t i = 0; i < numEnts; i++ )
		{
			entries.entityToEntityInScene[index][entries.entitiesInScene[index][i]] = i;
			Utilities::readString( stream, entries.entityNamesInScene[index][i] );
			s.stream.read( ( char* )&componentCount, sizeof( componentCount ) );
			ReadComponents( this, entries.entityNamesInScene[index][i], entries.entitiesInScene[index][i], stream, componentCount, managers );
			uint8_t bindToParent = 0;
			s.stream.read( ( char* )&bindToParent, sizeof( bindToParent ) );
			if ( bindToParent )
				initInfo.transformManager->BindChild( entity, entries.entitiesInScene[index][i], TransformFlags::INHERIT_ALL );
		}



	} );*/
}

uint64_t ECS::SceneManager::GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer )const noexcept
{
	/*PROFILE;
	if ( auto find = entries.find( entity ); !find.has_value() )
		return;
	else
	{
		uint64_t size = sizeof( version ) + sizeof( uint32_t );
		auto& entities = entries.peek<Entries::EntitiesInScene>( *find );
		auto& names = entries.peek<Entries::EntitiesInSceneNames>( *find );
		struct Component {
			Utilities::GUID type;
			std::function<bool( std::ostream& stream )> writer;
		};
		struct WriteInfo {
			std::string name;
			uint32_t componentCount;
			uint8_t bindToParent;
			std::vector<Component> eInfo;
		};
		WriteInfo myComponents{ entries.name[findf->second] };
		size += sizeof( uint32_t ) + static_cast< uint64_t >( myComponents.name.size() );
		std::vector<Utilities::GUID> childScenes;
		uint32_t numChildScenes = 0;
		for ( auto& e : entities )
			if ( auto findC = entityToEntry.find( e ); findC != entityToEntry.end() )
				childScenes.push_back( entries.name[findC->second] );
		size += sizeof( uint32_t ) + sizeof( Utilities::GUID ) * childScenes.size();

		size += sizeof( uint32_t );
		for ( auto m : managers )
		{
			if ( m == this )
				continue;
			if ( m->is_registered( entity ) )
			{
				Component comp{ m->GetManagerType() };

				auto writerSize = m->GetDataWriter( entity, comp.writer );
				if ( writerSize == 0 )
					continue;
				size += sizeof( comp.type );
				size += writerSize;
				myComponents.eInfo.push_back( std::move( comp ) );
			}

		}
		myComponents.componentCount = static_cast< uint32_t >( myComponents.eInfo.size() );


		std::vector<WriteInfo> writeInfo;
		writeInfo.reserve( entities.size() );
		for ( size_t i = 0; i < entities.size(); i++ )
		{
			auto& e = entities[i];
			WriteInfo info{ names[i], 0 };
			size += sizeof( uint32_t ) + static_cast< uint64_t >( info.name.size() );
			size += sizeof( uint32_t );
			if ( is_registered( e ) )
			{
				Component comp{ GetManagerType() };
				comp.writer = []( std::ostream* out )
				{
					return true;
				};


				size += sizeof( comp.type );
				info.eInfo.push_back( std::move( comp ) );
			}
			else
			{
				for ( auto m : managers )
				{

					if ( m->is_registered( e ) )
					{
						Component comp{ m->GetManagerType() };
						uint64_t writerSize = 0;

						writerSize = m->GetDataWriter( e, comp.writer );
						if ( writerSize == 0 )
							continue;

						size += sizeof( comp.type );

						size += writerSize;
						info.eInfo.push_back( std::move( comp ) );
					}

				}
			}

			info.componentCount = static_cast< uint32_t >( info.eInfo.size() );

			size += sizeof( info.bindToParent );
			info.bindToParent = uint8_t( initInfo.transformManager->GetParent( e ) == entity );
			writeInfo.push_back( std::move( info ) );


		}

		writer = [entity, this, writeInfo, myComponents, size, childScenes]( std::ostream* file )
		{

			if ( auto find = entityToEntry.find( entity ); find == entityToEntry.end() )
				return false;
			else
			{
				uint64_t start = file->tellp();
				auto& entities = entries.entitiesInScene[find->second];

				file->write( ( char* )&version, sizeof( version ) );
				Utilities::writeString( file, myComponents.name );

				uint32_t numCS = uint32_t( childScenes.size() );
				file->write( ( char* )&numCS, sizeof( numCS ) );
				file->write( ( char* )childScenes.data(), sizeof( Utilities::GUID ) * numCS );

				file->write( ( char* )&myComponents.componentCount, sizeof( myComponents.componentCount ) );
				for ( auto& comp : myComponents.eInfo )
				{
					file->write( ( char* )&comp.type, sizeof( comp.type ) );
					if ( !comp.writer( file ) )
						return false;
				}

				uint32_t numEnt = static_cast< uint32_t >( entities.size() );
				file->write( ( char* )&numEnt, sizeof( numEnt ) );
				for ( auto& info : writeInfo )
				{
					Utilities::writeString( file, info.name );
					file->write( ( char* )&info.componentCount, sizeof( info.componentCount ) );
					for ( auto& comp : info.eInfo )
					{
						file->write( ( char* )&comp.type, sizeof( comp.type ) );
						if ( !comp.writer( file ) )
							return false;
					}
					file->write( ( char* )&info.bindToParent, sizeof( info.bindToParent ) );
				}

				uint64_t asize = uint64_t( file->tellp() ) - start;
				if ( asize != size )
					return false;

				return true;
			}
		};



		return size;
	}*/
	return 0;
}



void ECS::SceneManager::Create( Entity entity, const std::string& name )noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); !find.has_value() )
		return;

	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;

	auto index = entries.add( entity );
	entries.set<Entries::Name>( index, name );

	initInfo.transformManager->Create( entity );
}
void ECS::SceneManager::AddEntityToScene( Entity scene, Entity entity, const std::string& name )noexcept
{
	PROFILE;
	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;

	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
	{
		auto& sceneMap = entries.get<Entries::EntityToEntityInScene>( *find );
		if ( auto findInScene = sceneMap.find( entity ); findInScene != sceneMap.end() )
		{
			auto& names = entries.get<Entries::EntitiesInSceneNames>( *find );
			names[findInScene->second] = name;
		}
		else
		{
			auto& entities = entries.get<Entries::EntitiesInScene>( *find );
			auto& names = entries.get<Entries::EntitiesInSceneNames>( *find );
			uint32_t index = static_cast< uint32_t >( entities.size() );
			entities.push_back( entity );
			names.push_back( name );
			sceneMap[entity] = index;
		}
		initInfo.transformManager->Create( entity );
		initInfo.transformManager->BindChild( scene, entity, TransformFlags::INHERIT_ALL );

	}
}
void ECS::SceneManager::AddEntityToScene( Entity scene, Entity entity )noexcept
{
	PROFILE;
	if ( !initInfo.entityManager->IsAlive( entity ) )
		return;

	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
	{
		auto& sceneMap = entries.get<Entries::EntityToEntityInScene>( *find );
		if ( auto findInScene = sceneMap.find( entity ); findInScene != sceneMap.end() )
			return;

		auto& entities = entries.get<Entries::EntitiesInScene>( *find );
		auto& names = entries.get<Entries::EntitiesInSceneNames>( *find );
		uint32_t index = static_cast< uint32_t >( entities.size() );
		entities.push_back( entity );
		if ( auto findChild = entries.find( entity ); !findChild.has_value() )
			names.push_back( entries.peek<Entries::Name>( *findChild ) );
		else
			names.push_back( "Entity_" + std::to_string( entity ) );
		sceneMap[entity] = index;

		initInfo.transformManager->Create( entity );
		initInfo.transformManager->BindChild( scene, entity, TransformFlags::INHERIT_ALL );
	}
}

void ECS::SceneManager::AddEntitiesToScene( Entity scene, const Entity entities[], uint32_t numEntities )noexcept
{
	PROFILE;

	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
	{
		auto& sceneMap = entries.get<Entries::EntityToEntityInScene>( *find );
		auto& entities = entries.get<Entries::EntitiesInScene>( *find );
		auto& names = entries.get<Entries::EntitiesInSceneNames>( *find );
		for ( uint32_t i = 0; i < numEntities; i++ )
		{
			if ( !initInfo.entityManager->IsAlive( entities[i] ) )
				continue;


			if ( auto findInScene = sceneMap.find( entities[i] ); findInScene != sceneMap.end() )
				continue;

			
			uint32_t index = static_cast< uint32_t >( entities.size() );
			entities.push_back( entities[i] );
			if ( auto findChild = entries.find( entities[i] ); !findChild.has_value() )
				names.push_back( entries.peek<Entries::Name>( *findChild ) );
			else
				names.push_back( "Entity_" + std::to_string( entities[i] ) );
			sceneMap[entities[i]] = index;
			initInfo.transformManager->Create( entities[i] );
			initInfo.transformManager->BindChild( scene, entities[i], TransformFlags::INHERIT_ALL );
		}
	}
}

uint32_t ECS::SceneManager::GetNumberOfEntitiesInScene( Entity scene )const noexcept
{
	PROFILE;

	if ( auto find = entries.find( scene ); !find.has_value() )
		return 0;
	else
	{
		return static_cast< uint32_t >( entries.peek<Entries::EntitiesInScene>( *find ).size());
	}
}

void ECS::SceneManager::GetEntitiesInScene( Entity scene, Entity entities[] )const noexcept
{
	PROFILE;

	if ( auto find = entries.find( scene ); !find.has_value() )
		return;
	else
	{
		memcpy( entities, entries.peek<Entries::EntitiesInScene>( *find ).data(), sizeof( Entity ) * entries.peek<Entries::EntitiesInScene>( *find ).size() );
	}
}

void ECS::SceneManager::Destroy( Entity entity )noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); !find.has_value() )
		return;
	else
	{
		initInfo.entityManager->DestroyMultiple( entries.peek<Entries::EntitiesInScene>( *find ).data(), uint32_t( entries.peek<Entries::EntitiesInScene>( *find ).size() ) );
		entries.erase( *find );
	}
}

void ECS::SceneManager::DestroyEntities( const Entity entities[], uint32_t numEntities )noexcept
{
	PROFILE;
	for ( uint32_t i = 0; i < numEntities; i++ )
	{
		if ( auto find = entries.find( entities[i] ); !find.has_value() )
			continue;
		else
		{
			initInfo.entityManager->DestroyMultiple( entries.peek<Entries::EntitiesInScene>( *find ).data(), uint32_t( entries.peek<Entries::EntitiesInScene>( *find ).size() ) );
			entries.erase( *find );
		}
	}
}

uint32_t ECS::SceneManager::GetNumberOfRegisteredEntities() const noexcept
{
	return static_cast< uint32_t >( entries.size() );
}

void ECS::SceneManager::GetRegisteredEntities( Entity entities[], uint32_t numEntities ) const noexcept
{
	PROFILE;
	if ( numEntities != entries.size() )
		return;
	memcpy( entities, entries.peek<Entries::Entity>( ).data(), sizeof( Entity ) * numEntities );
}

void ECS::SceneManager::Frame()noexcept
{
	PROFILE;
}

uint64_t ECS::SceneManager::get_memory_usage() const noexcept
{
	return uint64_t();
}

void ECS::SceneManager::shrink_to_fit() noexcept
{
	PROFILE;
	for ( size_t i = 0; i < entries.size(); i++ )
	{
		entries.get<Entries::EntitiesInScene>(i).shrink_to_fit();
		entries.get<Entries::EntitiesInSceneNames>( i ).shrink_to_fit();
	}

	entries.shrink_to_fit();
}



bool ECS::SceneManager::is_registered( Entity entity ) const noexcept
{
	return entries.find( entity ).has_value();
}

Utilities::GUID ECS::SceneManager::GetManagerType() const noexcept
{
	return "Scene";
}

void ECS::SceneManager::DestroyAll()noexcept
{
	PROFILE;
	for ( uint32_t i = 0; i < entries.size(); i++ )
		initInfo.entityManager->DestroyMultiple( entries.peek<Entries::EntitiesInScene>( i ).data(), uint32_t( entries.peek<Entries::EntitiesInScene>( i ).size() ) );

	entries.clear();
}

void ECS::SceneManager::ToggleActive( Entity entity, bool active )noexcept
{}

void ECS::SceneManager::ToggleActive( const Entity entities[], uint32_t numEntites, bool active )noexcept
{}

void ECS::SceneManager::write_to_stream( std::ostream& stream ) const
{}

void ECS::SceneManager::create_from_stream( std::istream& stream )
{}

void ECS::SceneManager::GarbageCollection()noexcept
{}
