#ifndef _ECS_ENTITY_MANAGER_H_
#define _ECS_ENTITY_MANAGER_H_
#include <EntityManager_Interface.h>
#include <vector>
#include <stack>
namespace ECS
{
	class EntityManager : public EntityManager_Interface {
	public:
		EntityManager();

		virtual Entity Create()noexcept override;
		virtual void CreateMultiple( Entity entities[], size_t numEntities )noexcept override;
		virtual void CreateMultiple( std::vector<Entity>& entities )noexcept override;
		virtual bool IsAlive( Entity entity )noexcept  override;
		virtual void Destroy( Entity entity )noexcept  override;
		virtual void DestroyMultiple( const Entity entities[], size_t numEntities )noexcept override;
		virtual void DestroyMultipleNow( const Entity entities[], size_t numEntities )noexcept override;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept override;
		virtual void DestroyMultipleNow( const std::vector<Entity>& entities )noexcept override;
		virtual void DestroyNow( Entity entity )noexcept  override;
		virtual void DestroyAll( bool immediate = false )noexcept  override;
		virtual size_t GetNumberOfAliveEntities()const  noexcept override;
		virtual void RegisterManagerForDestroyNow( std::weak_ptr<Manager_Base> manager )noexcept   override;
		virtual void UnregisterManagerForDestroyNow( Utilities::GUID manager )noexcept   override;


		virtual uint64_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit() noexcept override;
		virtual void write_to_stream( std::ostream& stream )const override;
		virtual void read_from_stream( std::istream& stream )override;
	private:
		std::vector<Entity::GENERATION_TYPE> generation;
		std::stack<decltype( Entity::id ) > freeIndices;

		std::vector<std::weak_ptr<Manager_Base>> managers;
	};
}
#endif
