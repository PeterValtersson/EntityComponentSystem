#include "DataManager.h"

#include <Profiler.h>

#ifdef _DEBUG
#pragma comment(lib, "ProfilerD.lib")
#else
#pragma comment(lib, "Profiler.lib")
#endif



namespace MPE
{
	DataManager::DataManager( threadIdentifier identifier, uint8_t frameSyncTime ) :
		Manager( identifier, frameSyncTime )
	{
		_Allocate( 10 );
	}

	DataManager::~DataManager()
	{
		for ( auto& e : _entityToIndex )
		{
			delete _entityEntires.dataBuff[e.second];
		}
		operator delete( _entityEntires.buffer );
	}
	const void DataManager::Start()
	{
		Msg msg;
		bool running = true;
		while ( running )
		{
			StartProfile;


			if ( PeekMsg( msg, Msg::Destination::Any, Msg::Tag::Any ) )
			{
				if ( msg.tag == Msg::Tag::Shutdown )
					running = false;
				if ( msg.tag == Msg::Tag::DataManager::RegisterEntity )
				{
					auto& entity = *( Entity* )msg.data;
					_CreateData( entity );
				}
			}

			std::this_thread::sleep_for( std::chrono::milliseconds( _frameSyncTime ) );

			StopProfile;
		}

		return void();
	}
	const void DataManager::_CreateData( const Entity& entity )
	{
		StartProfile;
		auto find = _entityToIndex.find( entity );
		if ( find != _entityToIndex.end() )
			ProfileReturnVoid;

		if ( _entityEntires.used >= _entityEntires.allocated )
			_Allocate( static_cast< uint32_t >( _entityEntires.allocated * 1.5f ) + 10 );

		//Utils::ConsoleLog::DumpToConsole("Creating Datacomponent for Entity: %d", entity.ID);

		uint32_t index = _entityToIndex[entity] = static_cast< uint32_t >( _entityToIndex.size() );
		_entityEntires.entity[index] = entity;
		_entityEntires.dataBuff[index] = new DataBuffer();
		_entityEntires.used++;

		ProfileReturnVoid;
	}
	//const void DataManager::AddBoolValue(const Entity & entity, const string & key, const bool val)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		size_t newheaderSize = static_cast<size_t>((header.entryCount + 1)*header.entrySize);
	//		size_t totalNewSize = newheaderSize + buffer.v_buffer.size;

	//		if (header.entryCount + 1 > header.capacity)
	//		{
	//			// Header alloc
	//			if (totalNewSize + static_cast<size_t>((header.capacity + 5)*header.entrySize) > buffer.capacity)
	//				buffer.AllocateAndResizeHeader(totalNewSize * 2 + 50);
	//			else
	//				buffer.HeaderResize();

	//		}
	//		else if (totalNewSize > buffer.capacity)
	//			buffer.Allocate(totalNewSize * 2 + 50);

	//		const uint32_t keyh = key.GetHash();

	//		header.keys[header.entryCount] = keyh;
	//		header.type[header.entryCount] = DataType::BOOL;
	//		header.value[header.entryCount].b = val;

	//		header.entryCount++;
	//	}

	//}
	//const void DataManager::AddFloatValue(const Entity & entity, const string & key, const float val)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		size_t newheaderSize = static_cast<size_t>((header.entryCount + 1)*header.entrySize);
	//		size_t totalNewSize = newheaderSize + buffer.v_buffer.size;

	//		if (header.entryCount + 1 > header.capacity)
	//		{
	//			// Header alloc
	//			if (totalNewSize + static_cast<size_t>((header.capacity + 5)*header.entrySize) > buffer.capacity)
	//				buffer.AllocateAndResizeHeader(totalNewSize * 2 + 50);
	//			else
	//				buffer.HeaderResize();

	//		}
	//		else if (totalNewSize > buffer.capacity)
	//			buffer.Allocate(totalNewSize * 2 + 50);

	//		const uint32_t keyh = key.GetHash();

	//		header.keys[header.entryCount] = keyh;
	//		header.type[header.entryCount] = DataType::FLOAT;
	//		header.value[header.entryCount].f = val;

	//		header.entryCount++;
	//	}
	//}
	//const void DataManager::AddStringValue(const Entity & entity, const string & key, const string & val)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		size_t newheaderSize = static_cast<size_t>((header.entryCount + 1)*header.entrySize);
	//		size_t totalNewSize = newheaderSize + buffer.v_buffer.size;

	//		if (header.entryCount + 1 > header.capacity)
	//		{
	//			// Header alloc
	//			if (totalNewSize + static_cast<size_t>((header.capacity + 5)*header.entrySize) > buffer.capacity)
	//				buffer.AllocateAndResizeHeader(totalNewSize * 2 + 50);
	//			else
	//				buffer.HeaderResize();

	//		}
	//		else if (totalNewSize > buffer.capacity)
	//			buffer.Allocate(totalNewSize * 2 + 50);

	//		const uint32_t keyh = key.GetHash();

	//		header.keys[header.entryCount] = keyh;
	//		header.type[header.entryCount] = DataType::STRING;

	//		Data& d = header.value[header.entryCount].data;
	//		d.size = static_cast<uint16_t>(val.size() * sizeof(char));
	//		buffer.v_buffer.size += static_cast<size_t>(d.size);
	//		d.offset = static_cast<uint16_t>(buffer.v_buffer.size);
	//		memcpy((void*)((size_t)buffer.data + buffer.capacity - static_cast<size_t>(d.offset)), val, d.size);

	//		header.entryCount++;
	//	}
	//}
	//const void DataManager::SetBoolValue(const Entity & entity, const string & key, const bool val)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		const uint32_t keyh = key.GetHash();
	//		for (uint8_t i = 0; i < header.entryCount; i++)
	//		{
	//			if (header.keys[i] == keyh)
	//			{
	//				if (header.type[i] != DataType::BOOL)
	//				{
	//					Utils::ConsoleLog::DumpToConsole("Tried to set bool value from entity data, but entry was not of type bool.");
	//					return;
	//				}
	//				header.value[i].b = val;
	//			}
	//		}
	//		Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
	//	}
	//}
	//const void DataManager::SetFloatValue(const Entity & entity, const string & key, const float val)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		const uint32_t keyh = key.GetHash();
	//		for (uint8_t i = 0; i < header.entryCount; i++)
	//		{
	//			if (header.keys[i] == keyh)
	//			{
	//				if (header.type[i] != DataType::FLOAT)
	//				{
	//					Utils::ConsoleLog::DumpToConsole("Tried to set float value from entity data, but entry was not of type float.");
	//					return;
	//				}
	//				header.value[i].f = val;
	//			}
	//		}
	//		Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
	//	}
	//}
	//const void DataManager::SetStringValue(const Entity & entity, const string & key, const string & val)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		auto& v_buffer = buffer.v_buffer;
	//		const uint32_t keyh = key.GetHash();
	//		for (uint8_t i = 0; i < header.entryCount; i++)
	//		{
	//			if (header.keys[i] == keyh)
	//			{
	//				if (header.type[i] != DataType::STRING)
	//				{
	//					Utils::ConsoleLog::DumpToConsole("Tried to get string value from entity data, but entry was not of type string.");
	//					return;
	//				}

	//				Data& d = header.value[i].data;
	//				uint16_t prevSize = d.size;
	//				d.size = static_cast<uint16_t>(val.size() * sizeof(char));
	//				if (d.size > prevSize)
	//				{
	//					v_buffer.size += static_cast<size_t>(d.size);
	//					d.offset = static_cast<uint16_t>(v_buffer.size);
	//				}
	//				else
	//					d.offset -= prevSize - d.size;
	//				memcpy((void*)((size_t)buffer.data + buffer.capacity - static_cast<size_t>(d.offset)), val, d.size);
	//			}
	//		}

	//		Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
	//	}
	//}
	//bool DataManager::GetBoolValue(const Entity & entity, const string & key, bool default_value)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		const uint32_t keyh = key.GetHash();
	//		for (uint8_t i = 0; i < header.entryCount; i++)
	//		{
	//			if (header.keys[i] == keyh)
	//			{
	//				if (header.type[i] != DataType::BOOL)
	//				{
	//					Utils::ConsoleLog::DumpToConsole("Tried to get bool value from entity data, but entry was not of type bool.");
	//					return false;
	//				}
	//				return header.value[i].b;
	//			}
	//		}
	//		Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
	//	}
	//	return default_value;
	//}
	//float DataManager::GetFloatValue(const Entity & entity, const string & key, float default_value)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		const uint32_t keyh = key.GetHash();
	//		for (uint8_t i = 0; i < header.entryCount; i++)
	//		{
	//			if (header.keys[i] == keyh)
	//			{
	//				if (header.type[i] != DataType::FLOAT)
	//				{
	//					Utils::ConsoleLog::DumpToConsole("Tried to get float value from entity data, but entry was not of type float.");
	//					return false;
	//				}
	//				return header.value[i].f;
	//			}
	//		}
	//		Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
	//	}
	//	return default_value;
	//}
	//string DataManager::GetStringValue(const Entity & entity, const string & key, string default_value)
	//{
	//	auto& find = _entityToIndex->find(entity);
	//	if (find != _entityToIndex->end())
	//	{
	//		auto& buffer = *_datap->dataBuff[find->second];
	//		auto& header = buffer.header;
	//		const uint32_t keyh = key.GetHash();
	//		for (uint8_t i = 0; i < header.entryCount; i++)
	//		{
	//			if (header.keys[i] == keyh)
	//			{
	//				if (header.type[i] != DataType::STRING)
	//				{
	//					Utils::ConsoleLog::DumpToConsole("Tried to get string value from entity data, but entry was not of type string.");
	//					return false;
	//				}
	//				Data& d = header.value[i].data;
	//				string s((char*)((size_t)buffer.data + buffer.capacity - static_cast<size_t>(d.offset)));
	//				s.Resize(d.size);

	//				return s;
	//			}
	//		}

	//		Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
	//	}
	//	return default_value;
	//}
	const void DataManager::_Allocate( uint32_t size )
	{
		StartProfile;
		if ( size <= _entityEntires.allocated )
		{
			printf( "Data Manager tried to shrink entity entry block.\n" );
			return;
		}
		EntityData new_data;
		size_t bytes = static_cast< size_t >( size * ( sizeof( Entity ) + sizeof( DataBuffer* ) ) );

		new_data.buffer = operator new( bytes );
		new_data.used = _entityEntires.used;
		new_data.allocated = size;

		new_data.entity = ( Entity* )( new_data.buffer );
		new_data.dataBuff = ( DataBuffer** )( new_data.entity + size );

		memcpy( new_data.entity, _entityEntires.entity, _entityEntires.used * sizeof( Entity ) );
		memcpy( new_data.dataBuff, _entityEntires.dataBuff, _entityEntires.used * sizeof( DataBuffer* ) );


		operator delete( _entityEntires.buffer );
		_entityEntires = new_data;

		StopProfile;
	}
	const void DataManager::_Destroy( uint32_t index )
	{
		StartProfile;
		uint32_t last = _entityEntires.used - 1;
		const Entity& e = _entityEntires.entity[index];
		const Entity& last_e = _entityEntires.entity[last];

		_entityEntires.entity[index] = _entityEntires.entity[last];
		delete _entityEntires.dataBuff[index];
		_entityEntires.dataBuff[index] = _entityEntires.dataBuff[last];

		_entityToIndex[last_e] = index;
		_entityToIndex.erase( e );

		_entityEntires.used--;
		StopProfile;
	}













	DataManager::DataBuffer::DataBuffer() : capacity( 0 ), data( nullptr )
	{
		AllocateAndResizeHeader( header.entrySize * 50 );
	}
	DataManager::DataBuffer::~DataBuffer()
	{
		operator delete( data );
	}

	const void DataManager::DataBuffer::Allocate( size_t size )
	{
		StartProfile;
		if ( size <= capacity )
		{
			printf( "Data Manager tried to shrink a Data Buffer.\n" );
			ProfileReturnVoid;
		}


		void* new_data = operator new( size );

		// Copy header data
		memcpy( new_data, data, header.capacity * header.entrySize );

		// Fix the pointers
		header.keys = ( uint32_t* )new_data;
		header.type = ( DataType* )( header.keys + header.capacity );
		header.value = ( Value* )( header.type + header.capacity );

		// Copy the string data
		memcpy( ( void* )( ( size_t )new_data + size - v_buffer.size ), ( void* )( ( size_t )data + capacity - v_buffer.size ), v_buffer.size );



		capacity = size;
		operator delete( data );
		data = new_data;

		ProfileReturnVoid;
	}
	const void DataManager::DataBuffer::AllocateAndResizeHeader( size_t size )
	{
		StartProfile;
		if ( size <= capacity )
		{
			printf( "Data Manager tried to shrink a Data Buffer.\n" );
			ProfileReturnVoid;
		}

		EntryHeader newHeader = header;
		void* new_data = operator new( size );
		if ( ( ( ( uint16_t )header.capacity ) * 2 + 5 ) > ( ( uint16_t )~0 >> 8 ) )
			newHeader.capacity = ~0;
		else
			newHeader.capacity += header.capacity + 5;

		// Setup new pointers
		newHeader.keys = ( uint32_t* )new_data;
		newHeader.type = ( DataType* )( newHeader.keys + newHeader.capacity );
		newHeader.value = ( Value* )( newHeader.type + newHeader.capacity );

		// Copy the data(in reverse order to avoid overwriting data)
		memcpy( newHeader.value, header.value, header.capacity * sizeof( Value ) );
		memcpy( newHeader.type, header.type, header.capacity * sizeof( DataType ) );
		memcpy( newHeader.keys, header.keys, header.capacity * sizeof( uint32_t ) );

		// Copy the string data
		memcpy( ( void* )( ( size_t )new_data + size - v_buffer.size ), ( void* )( ( size_t )data + capacity - v_buffer.size ), v_buffer.size );

		operator delete( data );
		data = new_data;

		capacity = size;

		ProfileReturnVoid;
	}
	const void DataManager::DataBuffer::HeaderResize()
	{
		StartProfile;
		EntryHeader newHeader = header;
		if ( ( ( uint16_t )header.capacity ) * 2 + 5 > UINT8_MAX )
			newHeader.capacity = ~0;
		else
			newHeader.capacity = header.capacity * 2 + 5;

		// Setup new pointers
		newHeader.keys = ( uint32_t* )data;
		newHeader.type = ( DataType* )( newHeader.keys + newHeader.capacity );
		newHeader.value = ( Value* )( newHeader.type + newHeader.capacity );

		// Copy the data(in reverse order to avoid overwriting data)
		memcpy( newHeader.value, header.value, header.capacity * sizeof( Value ) );
		memcpy( newHeader.type, header.type, header.capacity * sizeof( DataType ) );
		memcpy( newHeader.keys, header.keys, header.capacity * sizeof( uint32_t ) );
		ProfileReturnVoid;
	}

}
