#include "RenderableManager.h"

namespace ECS
{

	RenderableManager::RenderableManager(const RenderableManager_InitializationInfo& ii)
	{
	}


	RenderableManager::~RenderableManager()
	{
	}
	void RenderableManager::Create()
	{
	}
	bool RenderableManager::IsRegistered(Entity entity) const
	{
		return false;
	}
	void RenderableManager::CreateFromResource(Entity entity, ResourceHandler::Resource resource)
	{
	}
	void RenderableManager::CreateFromStream(Entity entity, std::istream * stream)
	{
	}
	uint64_t RenderableManager::GetDataWriter(Entity entity, std::function<bool(std::ostream*file)>& writer) const
	{
		return uint64_t();
	}
	void RenderableManager::Destroy(Entity entity)
	{
	}
	void RenderableManager::DestroyEntities(const Entity entities[], uint32_t numEntities)
	{
	}
	void RenderableManager::DestroyAll()
	{
	}
	uint32_t RenderableManager::GetNumberOfRegisteredEntities() const
	{
		return uint32_t();
	}
	void RenderableManager::GetRegisteredEntities(Entity entities[], uint32_t numEntities) const
	{
	}
	void RenderableManager::Frame()
	{
	}
	Utilz::GUID RenderableManager::GetManagerType() const
	{
		return Utilz::GUID();
	}

	void RenderableManager::CreateFromFile(std::ifstream & file)noexcept
	{
	}
	void RenderableManager::WriteToFile(std::ofstream & file) const noexcept
	{
	}
	void RenderableManager::ShrinkToFit()noexcept
	{
	}
	uint64_t RenderableManager::GetMemoryUsage() const noexcept
	{
		return uint64_t();
	}

	void RenderableManager::GarbageCollection()
	{
	}
}