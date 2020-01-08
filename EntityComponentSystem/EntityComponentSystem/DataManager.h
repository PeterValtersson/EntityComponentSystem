#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_
#pragma once
#include <Managers/DataManager_Interface.h>
#include <Utilities/Memory/Sofa.h>

namespace ECS
{

	class DataManager : public DataManager_Interface{
	public:
		/*DataManager Methods*/
		virtual void Create( Entity entity )noexcept override;
		virtual void CreateMultiple( const std::vector<Entity>& entities )noexcept override;

		virtual void CreateData( Entity entity, std::string_view key, Data data )noexcept override;
		virtual void SetData( Entity entity, Utilities::GUID key, Data data )noexcept override;
		virtual void RemoveData( Entity enitity, Utilities::GUID key )noexcept override;
		virtual Data GetData( Entity entity, Utilities::GUID key )const noexcept override;


		/* Manager Base Methods */
		virtual bool is_registered( Entity entity )const noexcept override;
		virtual void CreateFromResource( Entity entity, Resources::Resource resource ) override;
		virtual uint64_t GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer )const noexcept override;
		virtual void Destroy( Entity entity )noexcept override;
		virtual void DestroyMultiple( const Entity entities[], size_t numEntities )noexcept override;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept override;
		virtual void DestroyAll()noexcept override;

		virtual void ToggleActive( Entity entity, bool active )noexcept override;
		virtual void ToggleActive( const Entity entities[], size_t numEntities, bool active )noexcept override;
		virtual void ToggleActive( const std::vector<Entity>& entities, bool active )noexcept override;

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

	};

}
#endif
