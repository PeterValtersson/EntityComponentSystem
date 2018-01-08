#include "RenderableManager.h"

namespace ECS
{

	RenderableManager::RenderableManager(const RenderableManager_InitializationInfo& ii)
	{
	}


	RenderableManager::~RenderableManager()
	{
	}
	void RenderableManager::Create(Entity entity)noexcept
	{
	}
	bool RenderableManager::IsRegistered(Entity entity) const noexcept
	{
		return false;
	}
	void RenderableManager::CreateFromResource(Entity entity, ResourceHandler::Resource resource)noexcept
	{
	}
	void RenderableManager::CreateFromStream(Entity entity, std::istream * stream)noexcept
	{
	}
	uint64_t RenderableManager::GetDataWriter(Entity entity, std::function<bool(std::ostream*file)>& writer) const noexcept
	{
		return uint64_t();
	}
	void RenderableManager::Destroy(Entity entity)noexcept
	{
	}
	void RenderableManager::DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept
	{
	}
	void RenderableManager::DestroyAll()noexcept
	{
	}
	uint32_t RenderableManager::GetNumberOfRegisteredEntities() const noexcept
	{
		return uint32_t();
	}
	void RenderableManager::GetRegisteredEntities(Entity entities[], uint32_t numEntities) const noexcept
	{
	}
	void RenderableManager::Frame()noexcept
	{
	}
	Utilz::GUID RenderableManager::GetManagerType() const noexcept
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

	void RenderableManager::GarbageCollection()noexcept
	{
	}
}