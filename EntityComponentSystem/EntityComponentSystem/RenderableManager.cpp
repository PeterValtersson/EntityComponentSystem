#include "RenderableManager.h"
#include <Profiler.h>
using namespace ResourceHandler;
namespace ECS
{

	RenderableManager::RenderableManager(const RenderableManager_InitializationInfo& ii)
		:initInfo(ii)
	{
		if (!ii.renderer)
			THROW_ERROR("Renderable Manager must have a renderer");
		if (!ii.transformManager)
			THROW_ERROR("Renderable Manager must have a transform manager");
		if (!ii.entityManager)
			THROW_ERROR("Renderable Manager must have a entity manager");
	
		initInfo.transformManager->RegisterTransformUser(this);
	}


	RenderableManager::~RenderableManager()
	{
	}


	void RenderableManager::Create(Entity entity, ResourceHandler::Resource mesh, ResourceHandler::Resource defaultMesh, ResourceHandler::Resource shader, ResourceHandler::Resource defaultShader)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); find.has_value())
			return;
		
		size_t index = entries.add(entity);
		entries.get<EntryNames::Visible>(index) = false;
		entries.get<EntryNames::Mesh>(index) = mesh;
		entries.get<EntryNames::DefaultMesh>(index) = defaultMesh;
		entries.get<EntryNames::Shader>(index) = shader;
		entries.get<EntryNames::DefaultShader>(index) = defaultShader;
	}
	bool RenderableManager::IsRegistered(Entity entity) const noexcept
	{
		return entries.find(entity).has_value();
	}
	void RenderableManager::CreateFromResource(Entity entity, ResourceHandler::Resource resource)
	{
		StartProfile;
		if (auto find = entries.find(entity); find.has_value())
			return;
		resource.CheckIn();
		ResourceData<RenderableManager_ResourceInfo> info;
		
		size_t index = entries.add(entity);
		entries.get<EntryNames::Visible>(index) = false;

		auto status = resource.GetData(info);
		if (status != ResourceHandler::LoadStatus::LOADED)
		{
			THROW_ERROR_EX("Could not CreateFromResource, Could not load: ", resource.GUID());
		}
		entries.get<EntryNames::Mesh>(index) = info->mesh;
		entries.get<EntryNames::Mesh>(index).Reset();
		entries.get<EntryNames::DefaultMesh>(index) = info->defaultMesh;
		entries.get<EntryNames::DefaultMesh>(index).Reset();

		entries.get<EntryNames::Shader>(index) = info->shader;
		entries.get<EntryNames::Shader>(index).Reset();
		entries.get<EntryNames::DefaultShader>(index) = info->defaultShader;
		entries.get<EntryNames::DefaultShader>(index).Reset();
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
		StartProfile;
		ToggleActive(entity, false);
		entries.erase(entity);
	}
	void RenderableManager::DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept
	{
		StartProfile;
		for (uint32_t i = 0; i < numEntities; i++) {
			ToggleActive(entities[i], false);
			entries.erase(entities[i]);
		}
	}
	void RenderableManager::DestroyAll()noexcept
	{
		entries.clear();
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
				if (entries.get<EntryNames::Mesh>(find->second).PeekStatus() & LoadStatus::LOADED)
				{
					//initInfo.renderer->AddRenderJob();
				}
				else
				{
					entitiesToSwitchMesh.push_back(entity);
				}

				entries.get<EntryNames::Shader>(find->second).CheckIn();
				if (entries.get<EntryNames::Shader>(find->second).PeekStatus() & LoadStatus::LOADED)
				{
					//initInfo.renderer->AddRenderJob();
				}
				else
				{
					entitiesToSwitchShader.push_back(entity);
				}
			}
			else
			{
				entries.get<EntryNames::Mesh>(find->second).CheckOut();
				entries.get<EntryNames::Shader>(find->second).CheckOut();
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
					if (entries.get<EntryNames::Mesh>(find->second).PeekStatus() & LoadStatus::LOADED)
					{
						//initInfo.renderer->AddRenderJob();
					}
					else
					{
						entitiesToSwitchMesh.push_back(entities[i]);
					}

					entries.get<EntryNames::Shader>(find->second).CheckIn();
					if (entries.get<EntryNames::Shader>(find->second).PeekStatus() & LoadStatus::LOADED)
					{
						//initInfo.renderer->AddRenderJob();
					}
					else
					{
						entitiesToSwitchShader.push_back(entities[i]);
					}
				}
				else
				{
					entries.get<EntryNames::Mesh>(find->second).CheckOut();
					entries.get<EntryNames::Shader>(find->second).CheckOut();
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
	Utilities::GUID RenderableManager::GetManagerType() const noexcept
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
