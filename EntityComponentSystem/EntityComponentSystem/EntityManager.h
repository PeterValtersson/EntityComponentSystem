#ifndef _ECS_ENTITY_MANAGER_H_
#define _ECS_ENTITY_MANAGER_H_
#include <EntityManager_Interface.h>
#include <vector>
#include <stack>
namespace ECS
{
	class EntityManager : public EntityManager_Interface
	{
	public:
		EntityManager();

		virtual Entity Create()noexcept override;
		virtual void CreateMultiple(Entity entities[], uint32_t numEntities)noexcept override;
		virtual bool IsAlive(Entity entity)noexcept  override;
		virtual void Destroy(Entity entity)noexcept  override;
		virtual void DestroyMultiple(const Entity entities[], uint32_t numEntities)noexcept override;
		virtual void DestroyMultipleNow(const Entity entities[], uint32_t numEntities)noexcept override;
		virtual void DestroyNow(Entity entity)noexcept  override;
		virtual void DestroyAll(bool immediate = false)noexcept  override;
		virtual uint32_t GetNumberOfAliveEntities()const  noexcept override;
		virtual void RegisterManagerForDestroyNow(Manager_Base* manager)noexcept   override;
		virtual void UnregisterManagerForDestroyNow(Manager_Base* manager)noexcept   override;


		virtual uint64_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit() noexcept override;
		virtual void write_to_stream(std::ostream& stream)const override;
		virtual void create_from_stream(std::istream& stream )override;
	private:
		std::vector<Entity::GENERATION_TYPE> generation;
		std::stack<decltype(Entity::id) > freeIndices;
		std::vector<Manager_Base*> managers;
	};
}
#endif
