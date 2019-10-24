#ifndef _ECS_SCENE_MANAGER_H_
#define _ECS_SCENE_MANAGER_H_
#include <Managers\SceneManager_Interface.h>
#include <unordered_map>
namespace ECS
{
	class SceneManager : public SceneManager_Interface
	{
	public:
		SceneManager(SceneManager_InitializationInfo ii);
		virtual ~SceneManager();
		virtual	void Create(Entity entity,const std::string& name)noexcept override;
		virtual void AddEntityToScene(Entity scene, Entity entity)noexcept override;
		virtual void AddEntitiesToScene(Entity scene, const Entity entities[], uint32_t numEntities)noexcept override;
		virtual void AddEntityToScene(Entity scene, Entity entity, const std::string& name)noexcept override;
		virtual void RemoveEntityFromScene(Entity scene, Entity entity)noexcept override;
		virtual uint32_t GetNumberOfEntitiesInScene(Entity scene)const noexcept override;
		virtual void GetEntitiesInScene(Entity scene, Entity entities[])const noexcept override;
		virtual void RegisterManager(Manager_Base* manager)noexcept override;
		virtual void SetNameOfScene(Entity scene, const std::string& name)noexcept override;
		virtual const char* GetNameOfScene(Entity scene)const noexcept override;
		virtual void SetNameOfEntityInScene(Entity scene, Entity entity, const std::string& name)noexcept override;
		virtual const char* GetNameOfEntityInScene(Entity scene, Entity entity)const noexcept override;
		virtual uint32_t GetNumberOfChildResourcesOfSceneResource(ResourceHandler::Resource resource)const override;
		virtual void GetChildResourcesOfSceneResource(ResourceHandler::Resource resource, Utilities::GUID resources[], uint32_t num)const override;


		virtual void CreateFromResource(Entity entity, ResourceHandler::Resource resource) override;
		virtual void CreateFromStream(Entity entity, std::istream* stream)noexcept override;
		virtual uint64_t GetDataWriter(Entity entity, std::function<bool(std::ostream* file)>& writer)const noexcept override;

		virtual bool is_registered(Entity entity)const noexcept override;
		virtual void Destroy(Entity entity)noexcept override;
		virtual void DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept override;
		virtual void DestroyAll()noexcept override;

		virtual void ToggleActive(Entity entity, bool active)noexcept override;
		virtual void ToggleActive(const Entity entities[], uint32_t numEntites, bool active)noexcept override;

		virtual uint32_t GetNumberOfRegisteredEntities()const noexcept override;
		virtual void GetRegisteredEntities(Entity entities[], uint32_t numEntities)const noexcept override;

		virtual void Frame()noexcept override;
		virtual Utilities::GUID GetManagerType()const noexcept override;

		virtual uint64_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit()noexcept override;
		virtual void write_to_file(std::ofstream& file)const;
		virtual void create_from_file(std::ifstream& file);
	protected:
		virtual void GarbageCollection()noexcept override;

	private:
		uint32_t version = 0;
		std::vector<Manager_Base*> managers;

		SceneManager_InitializationInfo initInfo;

		struct SceneEntries
		{
			std::vector<Entity> entity;
			std::vector<std::string> name;
			std::vector<std::vector<Entity>> entitiesInScene;
			std::vector<std::vector<std::string>> entityNamesInScene;
			std::vector<std::unordered_map<Entity, uint32_t, Entity::Hasher>> entityToEntityInScene;
			//std::vector<bool> update;
		}entries;
		std::unordered_map<Entity, uint32_t, Entity::Hasher> entityToEntry;
	};
}
#endif
