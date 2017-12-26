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
		virtual bool IsAlive(Entity entity)noexcept  override;
		virtual void Destroy(Entity entity)noexcept  override;
		virtual void DestroyNow(Entity entity)noexcept  override;
		virtual void DestroyAll(bool immediate = false)noexcept  override;
		virtual uint32_t GetNumberOfAliveEntities()const  noexcept override;
		virtual void RegisterManagerForDestroyNow(Manager_Base* manager)noexcept   override;
		virtual void UnregisterManagerForDestroyNow(Manager_Base* manager)noexcept   override;


		virtual uint64_t GetMemoryUsage()const noexcept override;
		virtual void ShrinkToFit() noexcept override;
		virtual void WriteToFile(std::ofstream& file)const override;
		virtual void CreateFromFile(std::ifstream& file)override;
	private:
		std::vector<Entity::GENERATION_TYPE> generation;
		std::stack<decltype(Entity::id) > freeIndices;
		std::vector<Manager_Base*> managers;
	};
}
#endif