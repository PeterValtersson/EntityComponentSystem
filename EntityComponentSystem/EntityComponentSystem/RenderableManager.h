//#ifndef _ECS_RENDERABLE_MANAGER_H_
//#define _ECS_RENDERABLE_MANAGER_H_
//#include <Managers\RenderableManager_Interface.h>
//#include <Sofa.h>
//namespace ECS
//{
//	class RenderableManager : public RenderableManager_Interface
//	{
//	public:
//		RenderableManager(const RenderableManager_InitializationInfo& ii);
//		~RenderableManager();
//
//		virtual void Create(Entity entity, ResourceHandler::Resource mesh, ResourceHandler::Resource defaultMesh, ResourceHandler::Resource shader, ResourceHandler::Resource defaultShader)noexcept override;
//
//		virtual bool is_registered(Entity entity)const noexcept override;
//		virtual void CreateFromResource(Entity entity, ResourceHandler::Resource resource) override;
//		virtual void CreateFromStream(Entity entity, std::istream* stream)noexcept override;
//		virtual uint64_t GetDataWriter(Entity entity, std::function<bool(std::ostream* file)>& writer)const noexcept override;
//		virtual void Destroy(Entity entity)noexcept override;
//		virtual void DestroyEntities(const Entity entities[], size_t numEntities)noexcept override;
//		virtual void DestroyAll()noexcept override;
//
//		virtual void ToggleActive(Entity entity, bool active)noexcept override;
//		virtual void ToggleActive(const Entity entities[], uint32_t numEntites, bool active)noexcept override;
//
//		virtual uint32_t GetNumberOfRegisteredEntities()const noexcept override;
//		virtual void GetRegisteredEntities(Entity entities[], size_t numEntities)const noexcept override;
//
//		virtual void Frame()noexcept override;
//
//		virtual Utilities::GUID GetManagerType()const noexcept override;
//
//		virtual uint64_t get_memory_usage()const noexcept override;
//		virtual void shrink_to_fit()noexcept override;
//		virtual void write_to_file(std::ofstream& file)const noexcept override;
//		virtual void create_from_file(std::ifstream& file)noexcept override;
//
//		virtual void UpdateEntityTransforms(const Matrix transforms[], const Entity entities[], size_t numEntities)noexcept override;
//	protected:
//		virtual void GarbageCollection()noexcept override;
//
//		RenderableManager_InitializationInfo initInfo;
//		uint32_t version = 0;
//
//		Utilities::SofA::Array::SofA<
//			Entity, Entity::Hasher, // Entity
//			bool, //visible
//			ResourceHandler::Resource, // Mesh
//			ResourceHandler::Resource, // Default Mesh
//			ResourceHandler::Resource, // Shader
//			ResourceHandler::Resource // Default Shader
//		>entries;
//
//		struct EntryNames
//		{
//			enum EntryNamesEnum
//			{
//				Entity,
//				Visible,
//				Mesh,
//				DefaultMesh,
//				Shader,
//				DefaultShader
//			};
//		};
//
//		std::vector<Entity> entitiesToSwitchMesh;
//		std::vector<Entity> entitiesToSwitchShader;
//	};
//}
//#endif
