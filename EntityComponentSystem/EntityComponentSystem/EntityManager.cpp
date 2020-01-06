#include "EntityManager.h"
#include <Utilities/Profiler/Profiler.h>
#include <Managers\Manager_Base.h>

namespace ECS
{
	EntityManager::EntityManager()
	{}



	Entity EntityManager::Create()noexcept
	{
		PROFILE;
		if ( freeIndices.size() )
		{
			auto top = freeIndices.top();
			freeIndices.pop();
			return Entity( generation[top], top );
		}

		uint32_t index = static_cast< uint32_t >( generation.size() );
		generation.push_back( 0 );
		return Entity( 0, index );
	}
	void EntityManager::CreateMultiple( Entity entities[], size_t numEntities )noexcept
	{
		PROFILE;
		for ( size_t i = 0; i < numEntities; i++ )
		{
			if ( freeIndices.size() )
			{
				auto top = freeIndices.top();
				freeIndices.pop();
				entities[i] = Entity( generation[top], top );
			}

			uint32_t index = static_cast< uint32_t >( generation.size() );
			generation.push_back( 0 );
			entities[i] = Entity( 0, index );
		}
	}

	void EntityManager::CreateMultiple( std::vector<Entity>& entities ) noexcept
	{
		CreateMultiple( entities.data(), entities.size() );
	}

	bool EntityManager::IsAlive( Entity entity )noexcept
	{
		PROFILE;
		if ( generation.size() <= entity.Index() )
			return false;
		return generation[entity.Index()] == entity.Gen();
	}
	void EntityManager::Destroy( Entity entity )noexcept
	{
		PROFILE;
		if ( generation.size() <= entity.Index() )
			return;
		if ( generation[entity.Index()] == entity.Gen() )
		{
			generation[entity.Index()]++;
			freeIndices.push( entity.Index() );
		}
	}


	void EntityManager::DestroyMultiple( const Entity entities[], size_t numEntities )noexcept
	{
		PROFILE;
		for ( size_t i = 0; i < numEntities; i++ )
		{
			if ( generation.size() <= entities[i].Index() )
				continue;
			if ( generation[entities[i].Index()] == entities[i].Gen() )
			{
				generation[entities[i].Index()]++;
				freeIndices.push( entities[i].Index() );
			}
		}
	}
	void EntityManager::DestroyMultipleNow( const Entity entities[], size_t numEntities )noexcept
	{
		PROFILE;
		for ( auto m : managers )
			if ( auto ml = m.lock() )
				ml->DestroyMultiple( entities, numEntities );
		for ( size_t i = 0; i < numEntities; i++ )
		{
			if ( generation.size() <= entities[i].Index() )
				continue;

			if ( generation[entities[i].Index()] == entities[i].Gen() )
			{

				generation[entities[i].Index()]++;
				freeIndices.push( entities[i].Index() );
			}
		}
	}
	void EntityManager::DestroyMultiple( const std::vector<Entity>& entities ) noexcept
	{
		DestroyMultiple( entities.data(), entities.size() );
	}
	void EntityManager::DestroyMultipleNow( const std::vector<Entity>& entities ) noexcept
	{
		DestroyMultipleNow( entities.data(), entities.size() );
	}
	void EntityManager::DestroyNow( Entity entity )noexcept
	{
		PROFILE;
		if ( generation.size() <= entity.Index() )
			return;
		if ( generation[entity.Index()] == entity.Gen() )
		{
			for ( auto m : managers )
				if ( auto ml = m.lock() )
					ml->Destroy( entity );
			generation[entity.Index()]++;
			freeIndices.push( entity.Index() );
		}
	}
	void EntityManager::DestroyAll( bool immediate )noexcept
	{
		PROFILE;
		if ( immediate )
		{
			for ( auto m : managers )
				if ( auto ml = m.lock() )
					ml->DestroyAll();

		}
		generation.clear();
		freeIndices = std::stack<decltype( Entity::id )>();
	}
	size_t EntityManager::GetNumberOfAliveEntities() const noexcept
	{
		return  generation.size() - freeIndices.size();
	}
	void EntityManager::RegisterManagerForDestroyNow( std::weak_ptr<Manager_Base> manager )noexcept
	{
		PROFILE;
		if ( auto manager_lock = manager.lock() )
		{
			for ( auto& m : managers )
				if ( auto ml = m.lock(); ml && ml->GetManagerType() == manager_lock->GetManagerType() )
					return;
			managers.push_back( manager );
		}
	}
	void EntityManager::UnregisterManagerForDestroyNow( Utilities::GUID manager )noexcept
	{
		PROFILE;

		for ( size_t i = 0; i < managers.size(); i++ )
			if ( auto ml = managers[i].lock(); ml && ml->GetManagerType() == manager )
			{
				managers[i] = managers[managers.size() - 1];
				managers.pop_back();
				return;
			}


	}
	uint64_t EntityManager::get_memory_usage() const noexcept
	{
		PROFILE;
		uint64_t size = 0;
		size += generation.size() * sizeof( Entity::GENERATION_TYPE );
		size += freeIndices.size() * sizeof( decltype( Entity::id ) );
		size += managers.size() * sizeof( Manager_Base* );
		size += sizeof( *this );
		return  size;
	}
	void EntityManager::shrink_to_fit()noexcept
	{
		PROFILE;
		generation.shrink_to_fit();
		managers.shrink_to_fit();
	}
	void EntityManager::write_to_stream( std::ostream& stream ) const
	{
		PROFILE;
		uint32_t size = static_cast< uint32_t >( generation.size() );
		Utilities::Binary_Stream::write( stream, size );
		Utilities::Binary_Stream::write( stream, generation.data(), size * sizeof( Entity::GENERATION_TYPE ) );

		std::vector<decltype( Entity::id )> fi;
		auto tempInds = freeIndices;
		while ( !tempInds.empty() )
		{
			fi.push_back( tempInds.top() );
			tempInds.pop();
		}

		size = static_cast< uint32_t >( fi.size() );
		Utilities::Binary_Stream::write( stream, size );
		Utilities::Binary_Stream::write( stream, fi.data(), size * sizeof( decltype( Entity::id ) ) );
	}

	void EntityManager::read_from_stream( std::istream& stream )
	{
		PROFILE;
		uint32_t size = 0;
		Utilities::Binary_Stream::read( stream, size );
		generation.resize( size );
		Utilities::Binary_Stream::read( stream, generation.data(), size * sizeof( Entity::GENERATION_TYPE ) );

		Utilities::Binary_Stream::read( stream, size );
		std::vector<decltype( Entity::id )> fi;
		fi.resize( size );
		freeIndices = std::stack<decltype( Entity::id )>();
		Utilities::Binary_Stream::read( stream, fi.data(), size * sizeof( decltype( Entity::id ) ) );
		for ( auto i : fi )
			freeIndices.push( i );
	}

}
