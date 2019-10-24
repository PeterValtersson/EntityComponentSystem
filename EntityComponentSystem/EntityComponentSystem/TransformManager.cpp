#include "TransformManager.h"
#include <algorithm>
#include <Profiler.h>

#undef min
#undef max

namespace ECS
{
	XMFLOAT3 ToXMFLOAT3(const Vector& v)
	{
		return { v.x,  v.y, v.z };
	}
	Vector ToVector(const XMFLOAT3& f)
	{
		return { f.x, f.y, f.z };
	}
	
	TransformManager::TransformManager(ECS::TransformManager_InitializationInfo initInfo)
		: initInfo(initInfo)
	{
		_ASSERT_EXPR(initInfo.entityManager, L"Transform manager must have an entity manager");
		initInfo.entityManager->RegisterManagerForDestroyNow(this);
	}
	TransformManager::~TransformManager()
	{
		initInfo.entityManager->UnregisterManagerForDestroyNow(this);
	}





	void TransformManager::Create(Entity entity, const Vector & position, const Vector & rotaiton, const Vector & scale)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); find.has_value())
			return;

		if (!initInfo.entityManager->IsAlive(entity))
			return;

		auto index = entries.add(entity);
		entries.get<EntryNames::Position>(index) = ToXMFLOAT3(position);
		auto quat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3((XMFLOAT3*)&rotaiton));
		XMStoreFloat4(&entries.get<EntryNames::Rotation>(index), quat);
		entries.get<EntryNames::Scale>(index) = ToXMFLOAT3(scale);
		XMStoreFloat4x4(&entries.get<EntryNames::Transform>(index), XMMatrixIdentity());
		entries.get<EntryNames::Dirty>(index) = true;
		entries.get<EntryNames::Parent>(index) = -1;
		entries.get<EntryNames::Child>(index) = -1;
		entries.get<EntryNames::Sibling>(index) = -1;
		entries.get<EntryNames::Flags>(index) = TransformFlags::NONE;
	}
	void TransformManager::BindChild(Entity parent, Entity child, TransformFlags flags)noexcept
	{
		StartProfile;
		if (auto findParent = entries.find(parent); !findParent.has_value())
			return;
		else
		{
			if (auto findChild = entries.find(child); !findChild.has_value())
				return;
			else
			{
				entries.get<EntryNames::Dirty>(findChild->second) = true;
				entries.get<EntryNames::Parent>(findChild->second) = static_cast<uint32_t>(findParent->second);
				auto parentChild = entries.get<EntryNames::Child>(findParent->second);
				if (parentChild == -1)
				{
					entries.get<EntryNames::Child>(findParent->second) = static_cast<uint32_t>(findChild->second);
				}
				else
				{
					auto sibling = entries.get<EntryNames::Sibling>(parentChild);
					while (sibling != -1)
					{
						parentChild = sibling;
						sibling = entries.get<EntryNames::Sibling>(sibling);
					}
					entries.get<EntryNames::Sibling>(parentChild) = static_cast<uint32_t>(findChild->second);
				}
			}
		}

	}
	void TransformManager::GetChildren(Entity parent, Entity children[]) const noexcept
	{
		StartProfile;
		auto find = entries.find(parent);
		_ASSERT_EXPR(find.has_value(), L"Can't get children from non exitent parent");

		auto childIndex = entries.getConst<EntryNames::Child>(find->second);
		size_t i = 0;

		while(childIndex != -1)
		{
			children[i++] = entries.getConst<EntryNames::Entity>(childIndex);
			childIndex = entries.getConst<EntryNames::Sibling>(childIndex);
		}
	}

	uint32_t TransformManager::GetNumberOfChildren(Entity entity) const noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return 0;
		else
		{
			auto childIndex = entries.getConst<EntryNames::Child>(find->second);
			uint32_t i = 0;

			while (childIndex != -1)
			{
				i++;
				childIndex = entries.getConst<EntryNames::Sibling>(childIndex);
			}
			return i;
		}
	}

	Entity TransformManager::GetParent(Entity entity) const noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return -1;
		else
		{
			auto parentIndex = entries.getConst<EntryNames::Parent>(find->second);
			if (parentIndex == -1)
				return false;
			return entries.getConst<EntryNames::Entity>(parentIndex);
		}
	}

	void TransformManager::UnbindAllChildren(Entity entity, TransformFlags flags)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return;
		else
		{
			auto childIndex = entries.get<EntryNames::Child>(find->second);
			entries.get<EntryNames::Child>(find->second) = -1;
			while (childIndex != -1)
			{
				entries.get<EntryNames::Dirty>(childIndex) = true;
				entries.get<EntryNames::Parent>(childIndex) = -1;
				auto temp = entries.get<EntryNames::Sibling>(childIndex);
				entries.get<EntryNames::Sibling>(childIndex) = -1;
				childIndex = temp;
			}
		}
	}

	void TransformManager::UnbindParent(Entity entity, TransformFlags flags)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return;
		else
		{
			auto parentIndex = entries.get<EntryNames::Parent>(find->second);
			if (parentIndex == -1)
				return;

			entries.get<EntryNames::Parent>(find->second) = -1;
			entries.get<EntryNames::Dirty>(find->second) = true;
			auto childIndex = entries.get<EntryNames::Child>(parentIndex);
			if (childIndex == find->second)
			{
				entries.get<EntryNames::Child>(parentIndex) = entries.get<EntryNames::Sibling>(childIndex);
			}
			else
			{
				auto sibling = entries.get<EntryNames::Sibling>(childIndex);
				while (sibling != find->second)
				{
					childIndex = sibling;
					sibling = entries.get<EntryNames::Sibling>(childIndex);
				}

				entries.get<EntryNames::Sibling>(childIndex) = entries.get<EntryNames::Sibling>(sibling);
			}
		}
	}

	void TransformManager::SetPosition(Entity entity, const Vector & position)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return;
		else
		{
			entries.get<EntryNames::Position>(find->second) = ToXMFLOAT3(position);
			entries.get<EntryNames::Dirty>(find->second) = true;
		}
	}
	Vector TransformManager::GetPosition(Entity entity)const noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return Vector();
		else
			return ToVector(entries.getConst<EntryNames::Position>(find->second));
	}	
	void TransformManager::SetRotation(Entity entity, const Vector & rotation)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return;
		else
		{

			auto quat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3((XMFLOAT3*)&rotation));
			XMStoreFloat4(&entries.get<EntryNames::Rotation>(find->second), quat);
			entries.get<EntryNames::Dirty>(find->second) = true;
		}
	}
	Vector TransformManager::GetRotation(Entity entity) const noexcept
	{ 
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return Vector();
		else
		{
			//XMQuaternionToAxisAngle
			//	return ToVector(entries.getConst<EntryNames::Rotation>(find->second));
			return Vector();
		}
		
	}
	void TransformManager::SetScale(Entity entity, const Vector & scale)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return;
		else
		{
			entries.get<EntryNames::Scale>(find->second) = ToXMFLOAT3(scale);
			entries.get<EntryNames::Dirty>(find->second) = true;
		}
	}
	Vector TransformManager::GetScale(Entity entity) const noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return Vector();
		else
			return ToVector(entries.getConst<EntryNames::Scale>(find->second));
	}
	void TransformManager::SetTransform(Entity entity, const Matrix & transform)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return;
		else
		{
			XMMATRIX trans = XMLoadFloat4x4((XMFLOAT4X4*)&transform);
			XMVECTOR scale;
			XMVECTOR pos;
			XMVECTOR rot;
			XMMatrixDecompose(&scale, &rot, &pos, trans);
			XMStoreFloat3(&entries.get<EntryNames::Scale>(find->second), scale);
			XMStoreFloat3(&entries.get<EntryNames::Position>(find->second), pos);
			XMStoreFloat4(&entries.get<EntryNames::Rotation>(find->second), rot);
			entries.get<EntryNames::Dirty>(find->second) = true;
		}
		
	}
	Matrix TransformManager::GetTransform(Entity entity)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return Matrix();
		else
		{
			Matrix mat;
			XMStoreFloat4x4((XMFLOAT4X4*)&mat, XMLoadFloat4x4(&entries.getConst<EntryNames::Transform>(find->second)));
			return mat;
		}
		
	}

	void TransformManager::RegisterTransformUser(Manager_TransformUser * tUser)noexcept
	{
		StartProfile;
		for (auto tu : transformUsers)
			if (tu == tUser)
				return;

		transformUsers.push_back(tUser);
	}

	void TransformManager::UnregisterTransformUser(Manager_TransformUser * tUser)noexcept
	{
		StartProfile;
		for(auto& tu : transformUsers)
			if (tu == tUser)
			{
				tu = transformUsers[transformUsers.size() - 1];
				transformUsers.pop_back();
				break;
			}
	}



	void TransformManager::Destroy(Entity entity)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); !find.has_value())
			return;
		else
			entries.destroy(find->second);
	}
	void TransformManager::DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept
	{
		StartProfile;
		for (uint32_t i = 0; i < numEntities; i++)
		{
			if (auto find = entries.find(entities[i]); !find.has_value())
				return;
			else
				entries.destroy(find->second);
		}
	}
	uint64_t TransformManager::get_memory_usage() const noexcept
	{
		return entries.get_memory_usage() + sizeof(*this);
	}
	void TransformManager::shrink_to_fit()noexcept
	{
		StartProfile;
		entries.shrink_to_fit();
	}

	uint32_t TransformManager::GetNumberOfRegisteredEntities() const noexcept
	{
		return static_cast<uint32_t>(entries.size());
	}
	void TransformManager::GetRegisteredEntities(Entity entities[], uint32_t numEntities) const noexcept
	{
		if (numEntities != entries.size())
			return;
		memcpy(entities, entries.getConst<EntryNames::Entity>(), sizeof(Entity)*numEntities);
	}

	void TransformManager::Frame()noexcept
	{
		StartProfile;
		GarbageCollection();
		UpdateDirtyEntities();
	}

	uint64_t TransformManager::GetDataWriter(Entity entity, std::function<bool(std::ostream* file)>& writer)const noexcept
	{

		if (auto findF = entries.find(entity); !findF.has_value())
			return 0;


		writer = [this, entity](std::ostream* file) {
			if (auto find = entries.find(entity); !find.has_value())
				return false;
			else
			{
				file->write((char*)&version, sizeof(version));
				file->write((char*)&entries.getConst<EntryNames::Position>(find->second), sizeof(XMFLOAT3));
				file->write((char*)&entries.getConst<EntryNames::Rotation>(find->second), sizeof(XMFLOAT4));
				file->write((char*)&entries.getConst<EntryNames::Scale>(find->second), sizeof(XMFLOAT3));
				return true;
			}
		};

		return sizeof(XMFLOAT3) * 2 + sizeof(XMFLOAT4) + sizeof(version);



	}

	bool TransformManager::is_registered(Entity entity) const noexcept
	{
		return entries.find(entity).has_value();
	}

	void TransformManager::CreateFromResource(Entity entity, ResourceHandler::Resource resource)
	{
		struct Component
		{
			decltype(version) ver;
			XMFLOAT3 pos;
			XMFLOAT4 rot;
			XMFLOAT3 scale;
		};
	
		StartProfile;
		if (auto find = entries.find(entity); find.has_value())
			return;

		if (!initInfo.entityManager->IsAlive(entity))
			return;
	
		{
			ResourceHandler::ResourceData<Component> data(resource);
			auto index = entries.add(entity);

			entries.get<EntryNames::Position>(index) = data.Get().pos;
			entries.get<EntryNames::Rotation>(index) = data.Get().rot;
			entries.get<EntryNames::Scale>(index) = data.Get().scale;
			XMStoreFloat4x4(&entries.get<EntryNames::Transform>(index), XMMatrixIdentity());
			entries.get<EntryNames::Dirty>(index) = true;
			entries.get<EntryNames::Parent>(index) = -1;
			entries.get<EntryNames::Child>(index) = -1;
			entries.get<EntryNames::Sibling>(index) = -1;
			entries.get<EntryNames::Flags>(index) = TransformFlags::NONE;
		}
	}

	void TransformManager::CreateFromStream(Entity entity, std::istream * stream)noexcept
	{
		StartProfile;
		if (auto find = entries.find(entity); find.has_value())
			return;

		if (!initInfo.entityManager->IsAlive(entity))
			return;

		auto index = entries.add(entity);

		decltype(version) fileV = 0;
		stream->read((char*)&fileV, sizeof(fileV));
		stream->read((char*)&entries.getConst<EntryNames::Position>(index), sizeof(XMFLOAT3));
		stream->read((char*)&entries.getConst<EntryNames::Rotation>(index), sizeof(XMFLOAT4));
		stream->read((char*)&entries.getConst<EntryNames::Scale>(index), sizeof(XMFLOAT3));
		XMStoreFloat4x4(&entries.get<EntryNames::Transform>(index), XMMatrixIdentity());
		entries.get<EntryNames::Dirty>(index) = true;
		entries.get<EntryNames::Parent>(index) = -1;
		entries.get<EntryNames::Child>(index) = -1;
		entries.get<EntryNames::Sibling>(index) = -1;
		entries.get<EntryNames::Flags>(index) = TransformFlags::NONE;
	}

	Utilities::GUID TransformManager::GetManagerType() const noexcept
	{
		return "Transform";
	}

	void TransformManager::DestroyAll()noexcept
	{
		StartProfile;
		entries.clear();
	}

	void TransformManager::ToggleActive(Entity entity, bool active)noexcept
	{
	}

	void TransformManager::ToggleActive(const Entity entities[], uint32_t numEntites, bool active)noexcept
	{
	}

	void TransformManager::write_to_file(std::ofstream & file) const
	{
		StartProfile;
		file.write((char*)&version, sizeof(version));
		uint32_t used = static_cast<uint32_t>(entries.size());
		file.write((char*)&used, sizeof(used));
		file.write((char*)entries.getConst<EntryNames::Entity>(), sizeof(Entity)*used);
		file.write((char*)entries.getConst<EntryNames::Position>(), sizeof(XMFLOAT3)*used);
		file.write((char*)entries.getConst<EntryNames::Rotation>(), sizeof(XMFLOAT4)*used);
		file.write((char*)entries.getConst<EntryNames::Scale>(), sizeof(XMFLOAT3)*used);
		file.write((char*)entries.getConst<EntryNames::Parent>(), sizeof(uint32_t)*used);
		file.write((char*)entries.getConst<EntryNames::Child>(), sizeof(uint32_t)*used);
		file.write((char*)entries.getConst<EntryNames::Sibling>(), sizeof(uint32_t)*used);
	}
	void TransformManager::create_from_file(std::ifstream & file)
	{
		StartProfile;
		uint32_t ver = 0;
		file.read((char*)&ver, sizeof(ver));
		uint32_t used = 0;
		file.read((char*)&used, sizeof(used));
		entries.Allocate(used);
		file.read((char*)entries.get<EntryNames::Entity>(), sizeof(Entity)*used);
		file.read((char*)entries.getConst<EntryNames::Position>(), sizeof(XMFLOAT3)*used);
		file.read((char*)entries.getConst<EntryNames::Rotation>(), sizeof(XMFLOAT3)*used);
		file.read((char*)entries.getConst<EntryNames::Scale>(), sizeof(XMFLOAT3)*used);
		file.read((char*)entries.getConst<EntryNames::Parent>(), sizeof(uint32_t)*used);
		file.read((char*)entries.getConst<EntryNames::Child>(), sizeof(uint32_t)*used);
		file.read((char*)entries.getConst<EntryNames::Sibling>(), sizeof(uint32_t)*used);
		entries.Reinit(used);
		for (uint32_t i = 0; i < used; i++)
		{
			entries.get<EntryNames::Dirty>()[i] = true;
			XMStoreFloat4x4(&entries.get<EntryNames::Transform>()[i], XMMatrixIdentity());
		}
	}
	void TransformManager::UpdateDirtyEntities()noexcept
	{
		StartProfile;
		auto transforms = entries.getConst<EntryNames::Transform>();
		std::vector<uint32_t> entitiesToApplyParentTransformTo;
	
		for (size_t i = 0; i < entries.size(); i++)
		{
			if (entries.get<EntryNames::Dirty>(i))
			{
				const auto& translation = XMMatrixTranslationFromVector(XMLoadFloat3(&entries.get<EntryNames::Position>(i)));
				//const auto& qrot = XMMatrixRotationQuaternion()
				const auto& rotation = XMMatrixRotationQuaternion(XMLoadFloat4(&entries.get<EntryNames::Rotation>(i)));
				const auto& scale = XMMatrixScalingFromVector(XMLoadFloat3(&entries.get<EntryNames::Scale>(i)));
				XMStoreFloat4x4(&transforms[i], scale*rotation*translation);
				//XMMatrixTransformation()
				if (entries.get<EntryNames::Parent>(i) == -1)
				{
					auto child = entries.get<EntryNames::Child>(i);
					while (child != -1)
					{
						entitiesToApplyParentTransformTo.push_back(child);
						child = entries.get<EntryNames::Sibling>(child);
					}
				}
			}
		}
	
		do
		{
			auto entitiesToApplyTransTo = std::move(entitiesToApplyParentTransformTo);
			for (auto i : entitiesToApplyTransTo)
			{
				const auto& parentTransform = XMLoadFloat4x4(&transforms[entries.get<EntryNames::Parent>(i)]);
				const auto& childTransform = XMLoadFloat4x4(&transforms[i]);

				XMStoreFloat4x4((XMFLOAT4X4*)&transforms[i], childTransform*parentTransform);
				auto child = entries.get<EntryNames::Child>(i);
				while (child != -1)
				{
					entitiesToApplyParentTransformTo.push_back(child);
					child = entries.get<EntryNames::Sibling>(child);
				}
			}
		} while (entitiesToApplyParentTransformTo.size());

		auto entities = entries.get<EntryNames::Entity>();
		for(auto tu : transformUsers)
			tu->UpdateEntityTransforms((Matrix*)transforms, entities, static_cast<uint32_t>(entries.size()));
	}
	void TransformManager::GarbageCollection()noexcept
	{
		StartProfile;
		uint32_t alive_in_row = 0;
		const uint32_t quitWhenReached = std::max(uint32_t(entries.size() * 0.1f), 40U);
		while (entries.size() > 0 && alive_in_row < quitWhenReached)
		{
			std::uniform_int_distribution<size_t> distribution(0U, entries.size() - 1U);
			size_t i = distribution(generator);
			if (initInfo.entityManager->IsAlive(entries.get<EntryNames::Entity>(i)))
			{
				alive_in_row++;
				continue;
			}
			alive_in_row = 0;
			if (entries.get<EntryNames::Parent>(i) != -1)
			{
				UnbindParent(entries.get<EntryNames::Entity>(i), TransformFlags::NONE);
			}
			entries.destroy(i);
		}
	}
}
