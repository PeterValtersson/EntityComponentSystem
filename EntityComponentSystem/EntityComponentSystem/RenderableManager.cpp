#include "RenderableManager.h"
#include <Profiler.h>
using namespace ResourceHandler;
namespace ECS
{

	RenderableManager::RenderableManager(const RenderableManager_InitializationInfo& ii)
		:initInfo(ii)
	{
		_ASSERT_EXPR(ii.entityManager, L"RenderableManager must have a entity manager");
		_ASSERT_EXPR(ii.transformManager, L"RenderableManager must have a transform manager");
		_ASSERT_EXPR(ii.renderer, L"RenderableManager must have a renderer");
		initInfo.transformManager->RegisterTransformUser(this);
	}


	RenderableManager::~RenderableManager()
	{
	}


	void RenderableManager::Create(Entity entity)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); find.has_value())
			return;
		
		size_t index = entries.add(entity);
		entries.get<EntryNames::Visible>(index) = false;
		entries.get<EntryNames::Mesh>(index) = Resource("DefaultMesh", "StaticMesh");
	}
	bool RenderableManager::IsRegistered(Entity entity) const noexcept
	{
		return entries.find(entity).has_value();
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
	void RenderableManager::ToggleActive(Entity entity, bool active)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); find.has_value())
		{
			if (entries.get<EntryNames::Visible>(find->second) == active)
				return;

			entries.get<EntryNames::Visible>(find->second) = active;

			if (active)
			{
				entries.get<EntryNames::Mesh>(find->second).CheckIn();
				//if(entries.get<EntryNames::Mesh>(find->second).PeekStatus() & LoadStatus::LOADED)
				//initInfo.renderer->AddRenderJob();
			}
			else
			{
				entries.get<EntryNames::Mesh>(find->second).CheckOut();
				//initInfo.renderer->RemoveRenderJob();
			}
		}
	}

	void RenderableManager::ToggleActive(const Entity entities[], uint32_t numEntites, bool active)noexcept
	{
		StartProfile;
		for (uint32_t i = 0; i < numEntites; ++i)
		{
			if (auto find = entries.find(entities[i]); find.has_value())
			{
				if (entries.get<EntryNames::Visible>(find->second) == active)
					continue;

				entries.get<EntryNames::Visible>(find->second) = active;

				if (active)
				{
					entries.get<EntryNames::Mesh>(find->second).CheckIn();
					//if(entries.get<EntryNames::Mesh>(find->second).PeekStatus() & LoadStatus::LOADED)
					//initInfo.renderer->AddRenderJob();
				}
				else
				{
					entries.get<EntryNames::Mesh>(find->second).CheckOut();
					//initInfo.renderer->RemoveRenderJob();
				}
			}
		}
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
		return "Renderable";
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
	void RenderableManager::UpdateEntityTransforms(const Matrix transforms[], const Entity entities[], uint32_t numEntities)noexcept
	{
	}
	void RenderableManager::GarbageCollection()noexcept
	{
	}
}