#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_
#pragma once
#include <Managers/DataManager_Interface.h>
#include <Utilities/Memory/Sofa.h>

namespace ECS
{
	//! The DataManager is used to associate basic datatypes to an entity.
	/*!
	By sending a message to the data manager you can add one or more data entries to an entity.
	Each entry contains one DataBuffer. In the beginning of the DataBuffer the EntryHeader is located(which contains the Value for each DataType entry. The strings are stored at the end of the DataBuffer.
	\sa DataType
	\sa Tag::DataManager
	*/
	class DataManager : public DataManager_Interface {
	private:
		//! For the kinds of data that can be stored as an entry.
		enum class DataType : uint8_t {
			BOOL, FLOAT, STRING
		};

		//! Used for indexing the strings and other growing datatypes in the value_buffer.
		struct Data {
			uint16_t offset;
			uint16_t size;
		};

		//! The value struct, this is where the data is stored.
		/*!
		 The union clumps the memorylocation of all the members to the same position.
		 They share the same spot.
		*/
		struct Value {
			union {
				bool b;
				float f;
				Data data; // str
			};
		};

		//! The header struct, used for keeping track of the data entries an Entity has.
		struct Entries : public Utilities::Memory::SofA<
			Entity,
			uint8_t,
			uint8_t,
			DataType,
			Value> {
			enum Keys {
				Entity,
				Capacity,
				NumEntries,
				Type,
				Value
			};
		}entries;

		//struct EntryHeader {
		//	uint8_t capacity = 0;
		//	uint8_t entryCount = 0;
		//	static const uint8_t entrySize = sizeof( uint32_t ) + sizeof( DataType ) + sizeof( Value );

		//	uint32_t* keys;
		//	DataType* type;
		//	Value* value;
		//};

		//! Points to the next free spot in the value_buffer.
		struct Value_Buffer {
			size_t size = 0;
		};

		//! Struct for keeping track of the data entries an Entity has been given.
		/*!
		The databuffer stores the header on the left side and the value_buffer on the right side.
		 When the buffers meet the size is increased.
		*/
		struct DataBuffer {
			void* data;
			size_t capacity; /*!< This capacity is in bytes!!! */

			EntryHeader header;
			Value_Buffer v_buffer;

			DataBuffer();
			~DataBuffer();

			//! Grow the entry buffer.
			const void Allocate( size_t size );

			//! Grow the entry buffer and copy the header immediately.
			const void AllocateAndResizeHeader( size_t size );

			//! Grow the header.
			const void HeaderResize();
		};

		//! The managers Entity entry block.
		struct EntityData : public BaseManagerEntityEntryBlock {
			DataBuffer** dataBuff;/*!< Stores a pointer to the data entries for the Entity entry */
		};

		EntityData _entityEntires; /*!< A reference pointer to avoid having to cast the basic datapointer all the time. */

		std::unordered_map<Entity, uint32_t, EntityHasher> _entityToIndex;

	public:
		DataManager( threadIdentifier identifier, uint8_t frameSyncTime = 16 );
		~DataManager();

		//! The main entry point for the thread.
		const void Start();


	private:
		//! Register an Entity entry
		/*!
		An Entity need to be registered before any data can be associated with the Entity.
		*/
		const void _CreateData( const Entity& entity );

		//! Allocate more memory
		const void _Allocate( uint32_t size );

		//! Delete an entry in the memory block.
		/*!
		The deleted entry is replaced by the last in the block.
		*/
		const void _Destroy( uint32_t index );
	};




}
#endif
