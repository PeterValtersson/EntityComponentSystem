#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_
#pragma once
#include <Managers/PropertyManager_Interface.h>
#include <Utilities/Memory/Sofa.h>

namespace ECS
{

	class PropertyManager : public PropertyManager_Interface{
	public:
		PropertyManager( PropertyManager_Init_Info init_info );

		/*PropertyManager Methods*/
		virtual void Create( Entity entity )noexcept override;
		virtual void CreateMultiple( const std::vector<Entity>& entities )noexcept override;

		virtual void CreateProperty( Entity entity, std::string_view key, EntityProperty data )noexcept override;
		virtual void SetProperty( Entity entity, Utilities::GUID key, EntityProperty data )noexcept override;
		virtual void RemoveProperty( Entity entity, Utilities::GUID key )noexcept override;

		virtual EntityProperty GetProperty( Entity entity, Utilities::GUID key )const noexcept override;
		virtual std::string GetPropertyKey( Entity entity, Utilities::GUID key )const noexcept override;
		virtual size_t GetNumProperties( Entity entity )const noexcept override;

		/* Manager Base Methods */
		virtual bool is_registered( Entity entity )const noexcept override;
		virtual void CreateFromResource( Entity entity, ResourceHandler::Resource resource ) override;
		virtual uint64_t GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer )const noexcept override;
		virtual void Destroy( Entity entity )noexcept override;
		virtual void DestroyMultiple( const Entity entities[], size_t numEntities )noexcept override;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept override;
		virtual void DestroyAll()noexcept override;

		virtual size_t GetNumberOfRegisteredEntities()const noexcept override;
		virtual void GetRegisteredEntities( Entity entities[], size_t numEntities )const noexcept override;
		virtual std::vector<Entity> GetRegisteredEntities()const noexcept override;

		virtual void Frame()noexcept override;

		virtual Utilities::GUID GetManagerType()const noexcept override;


		/* Memory Base Methods*/
		virtual size_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit() override;
		virtual void write_to_stream( std::ostream& stream )const override;
		virtual void read_from_stream( std::istream& stream ) override;
	private:

		/* Manager Base Methods */
		virtual void GarbageCollection()noexcept override;



	private:
		PropertyManager_Init_Info init_info;
		std::default_random_engine generator;

		struct EntityProperties : public Utilities::Memory::SofV<
			Utilities::GUID, Utilities::GUID::Hasher,
			std::string,
			EntityProperty>{
			enum{
				Key,
				KeyString,
				Property
			};
		};

		struct Entries : public Utilities::Memory::SofV<
			Entity, Entity::Hasher,
			EntityProperties>{
			enum{
				Entity,
				Properties
			};
		} entries;
	};

}
#endif
