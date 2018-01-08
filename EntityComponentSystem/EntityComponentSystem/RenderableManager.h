#ifndef _ECS_RENDERABLE_MANAGER_H_
#define _ECS_RENDERABLE_MANAGER_H_
#include <Managers\RenderableManager_Interface.h>
namespace ECS
{
	class RenderableManager : public RenderableManager_Interface
	{
	public:
		RenderableManager(const RenderableManager_InitializationInfo& ii);
		~RenderableManager();

		virtual void Create()override;

		virtual bool IsRegistered(Entity entity)const override;
		virtual void CreateFromResource(Entity entity, ResourceHandler::Resource resource)override;
		virtual void CreateFromStream(Entity entity, std::istream* stream)override;
		virtual uint64_t GetDataWriter(Entity entity, std::function<bool(std::ostream* file)>& writer)const override;
		virtual void Destroy(Entity entity)override;
		virtual void DestroyEntities(const Entity entities[], uint32_t numEntities)override;
		virtual void DestroyAll()override;

		virtual uint32_t GetNumberOfRegisteredEntities()const override;
		virtual void GetRegisteredEntities(Entity entities[], uint32_t numEntities)const override;

		virtual void Frame()override;

		virtual Utilz::GUID GetManagerType()const override;

		virtual uint64_t GetMemoryUsage()const noexcept override;
		virtual void ShrinkToFit()noexcept override;
		virtual void WriteToFile(std::ofstream& file)const noexcept override;
		virtual void CreateFromFile(std::ifstream& file)noexcept override;

	protected:
		virtual void GarbageCollection()override;
	};
}
#endif