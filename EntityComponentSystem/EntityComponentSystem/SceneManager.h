#ifndef _ECS_SCENE_MANAGER_H_
#define _ECS_SCENE_MANAGER_H_
#include <Managers\SceneManager_Interface.h>
#include <unordered_map>
#include <Utilities/Memory/Sofa.h>

namespace ECS
{
	class SceneManager : public SceneManager_Interface {
	public:
		SceneManager( SceneManager_Init_Info ii );
		virtual ~SceneManager();
		virtual	void Create( Entity entity, const std::string& name )noexcept override;
		virtual void AddEntityToScene( Entity scene, Entity entity )noexcept override;
		virtual void AddEntitiesToScene( Entity scene, const Entity entities[], size_t numEntities )noexcept override;
		virtual void AddEntitiesToScene( Entity scene, const std::vector<Entity>& entities )noexcept override;
		virtual void AddEntityToScene( Entity scene, Entity entity, const std::string& name )noexcept override;
		virtual void RemoveEntityFromScene( Entity scene, Entity entity )noexcept override;
		virtual size_t GetNumberOfEntitiesInScene( Entity scene )const noexcept override;
		virtual void GetEntitiesInScene( Entity scene, Entity entities[], size_t numEntities )const noexcept override;
		virtual std::vector<Entity> GetEntitiesInScene( Entity scene )const noexcept override;
		virtual void RegisterManager( Manager_Base* manager )noexcept override;
		virtual void SetNameOfScene( Entity scene, const std::string& name )noexcept override;
		virtual std::string GetNameOfScene( Entity scene )const noexcept override;
		virtual void SetNameOfEntityInScene( Entity scene, Entity entity, const std::string& name )noexcept override;
		virtual std::string GetNameOfEntityInScene( Entity scene, Entity entity )const noexcept override;
		virtual size_t GetNumberOfChildResourcesOfSceneResource( ResourceHandler::Resource resource )const override;
		virtual void GetChildResourcesOfSceneResource( ResourceHandler::Resource resource, Utilities::GUID resources[], uint32_t num )const override;


		virtual void CreateFromResource( Entity entity, ResourceHandler::Resource resource ) override;
		virtual uint64_t GetDataWriter( Entity entity, std::function<bool( std::ostream& stream )>& writer )const noexcept override;

		virtual bool is_registered( Entity entity )const noexcept override;
		virtual void Destroy( Entity entity )noexcept override;
		virtual void DestroyMultiple( const Entity entities[], size_t numEntities )noexcept override;
		virtual void DestroyMultiple( const std::vector<Entity>& entities )noexcept override;
		virtual void DestroyAll()noexcept override;

		virtual size_t GetNumberOfRegisteredEntities()const noexcept override;
		virtual void GetRegisteredEntities( Entity entities[], size_t numEntities )const noexcept override;
		virtual std::vector<Entity> GetRegisteredEntities()const noexcept override;

		virtual void Frame()noexcept override;
		virtual Utilities::GUID GetManagerType()const noexcept override;

		virtual size_t get_memory_usage()const noexcept override;
		virtual void shrink_to_fit() override;
		virtual void write_to_stream( std::ostream& stream )const override;
		virtual void read_from_stream( std::istream& stream )override;
	protected:
		virtual void GarbageCollection()noexcept override;

	private:
		uint32_t version = 0;
		std::vector<Manager_Base*> managers;
		std::default_random_engine generator;
		SceneManager_Init_Info initInfo;

		struct Entries : public Utilities::Memory::SofV<
			Entity, Entity::Hasher,
			std::string,
			std::vector<Entity>,
			std::vector<std::string>,
			std::unordered_map<Entity, uint32_t, Entity::Hasher >>
		{
			enum Keys {
				Entity,
				Name,
				EntitiesInScene,
				EntitiesInSceneNames,
				EntityToEntityInScene
			};
		}entries;
		//struct SceneEntries {
		//	std::vector<Entity> entity;
		//	std::vector<std::string> name;
		//	std::vector<std::vector<Entity>> entitiesInScene;
		//	std::vector<std::vector<std::string>> entityNamesInScene;
		//	std::vector<std::unordered_map<Entity, uint32_t, Entity::Hasher>> entityToEntityInScene;
		//	//std::vector<bool> update;
		//}entries;
		//std::unordered_map<Entity, uint32_t, Entity::Hasher> entityToEntry;
	};
}
#endif
