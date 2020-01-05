#include "TransformManager.h"
#include <algorithm>
#include <Utilities/Profiler/Profiler.h>

#undef min
#undef max

namespace ECS
{
	XMFLOAT3 ToXMFLOAT3( const Vector& v )
	{
		return { v.x,  v.y, v.z };
	}
	Vector ToVector( const XMFLOAT3& f )
	{
		return { f.x, f.y, f.z };
	}

	TransformManager::TransformManager( ECS::TransformManager_Init_Info initInfo )
		: initInfo( initInfo )
	{
		_ASSERT_EXPR( initInfo.entityManager, L"Transform manager must have an entity manager" );
		initInfo.entityManager->RegisterManagerForDestroyNow( this );
	}
	TransformManager::~TransformManager()
	{
		initInfo.entityManager->UnregisterManagerForDestroyNow( this );
	}





	void TransformManager::Create( Entity entity, const Vector& position, const Vector& rotaiton, const Vector& scale )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); find.has_value() )
			return;

		if ( !initInfo.entityManager->IsAlive( entity ) )
			return;

		auto index = entries.add( entity );
		entries.get<Entries::Position>( index ) = ToXMFLOAT3( position );
		auto quat = XMQuaternionRotationRollPitchYawFromVector( XMLoadFloat3( ( XMFLOAT3* )&rotaiton ) );
		XMStoreFloat4( &entries.get<Entries::Rotation>( index ), quat );
		entries.get<Entries::Scale>( index ) = ToXMFLOAT3( scale );
		XMStoreFloat4x4( &entries.get<Entries::Transform>( index ), XMMatrixIdentity() );
		entries.get<Entries::Dirty>( index ) = true;
		entries.get<Entries::Parent>( index ) = -1;
		entries.get<Entries::Child>( index ) = -1;
		entries.get<Entries::Sibling>( index ) = -1;
		entries.get<Entries::Flags>( index ) = TransformFlags::NONE;
	}
	void TransformManager::BindChild( Entity parent, Entity child, TransformFlags flags )noexcept
	{
		PROFILE;
		if ( auto findParent = entries.find( parent ); !findParent.has_value() )
			return;
		else
		{
			if ( auto findChild = entries.find( child ); !findChild.has_value() )
				return;
			else
			{
				entries.get<Entries::Dirty>( *findChild ) = true;
				entries.get<Entries::Parent>( *findChild ) = static_cast< uint32_t >( *findParent );
				auto parentChild = entries.get<Entries::Child>( *findParent );
				if ( parentChild == -1 )
				{
					entries.get<Entries::Child>( *findParent ) = static_cast< uint32_t >( *findChild );
				}
				else
				{
					auto sibling = entries.get<Entries::Sibling>( parentChild );
					while ( sibling != -1 )
					{
						parentChild = sibling;
						sibling = entries.get<Entries::Sibling>( sibling );
					}
					entries.get<Entries::Sibling>( parentChild ) = static_cast< uint32_t >( *findChild );
				}
			}
		}

	}
	void TransformManager::GetChildren( Entity parent, Entity children[] ) const noexcept
	{
		PROFILE;
		auto find = entries.find( parent );
		_ASSERT_EXPR( find.has_value(), L"Can't get children from non exitent parent" );

		auto childIndex = entries.peek<Entries::Child>( *find );
		size_t i = 0;

		while ( childIndex != -1 )
		{
			children[i++] = entries.peek<Entries::Entity>( childIndex );
			childIndex = entries.peek<Entries::Sibling>( childIndex );
		}
	}

	uint32_t TransformManager::GetNumberOfChildren( Entity entity ) const noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return 0;
		else
		{
			auto childIndex = entries.peek<Entries::Child>( *find );
			uint32_t i = 0;

			while ( childIndex != -1 )
			{
				i++;
				childIndex = entries.peek<Entries::Sibling>( childIndex );
			}
			return i;
		}
	}

	Entity TransformManager::GetParent( Entity entity ) const noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return -1;
		else
		{
			auto parentIndex = entries.peek<Entries::Parent>( *find );
			if ( parentIndex == -1 )
				return false;
			return entries.peek<Entries::Entity>( parentIndex );
		}
	}

	void TransformManager::UnbindAllChildren( Entity entity, TransformFlags flags )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return;
		else
		{
			auto childIndex = entries.get<Entries::Child>( *find );
			entries.get<Entries::Child>( *find ) = -1;
			while ( childIndex != -1 )
			{
				entries.get<Entries::Dirty>( childIndex ) = true;
				entries.get<Entries::Parent>( childIndex ) = -1;
				auto temp = entries.get<Entries::Sibling>( childIndex );
				entries.get<Entries::Sibling>( childIndex ) = -1;
				childIndex = temp;
			}
		}
	}

	void TransformManager::UnbindParent( Entity entity, TransformFlags flags )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return;
		else
		{
			auto parentIndex = entries.get<Entries::Parent>( *find );
			if ( parentIndex == -1 )
				return;

			entries.get<Entries::Parent>( *find ) = -1;
			entries.get<Entries::Dirty>( *find ) = true;
			auto childIndex = entries.get<Entries::Child>( parentIndex );
			if ( childIndex == *find )
			{
				entries.get<Entries::Child>( parentIndex ) = entries.get<Entries::Sibling>( childIndex );
			}
			else
			{
				auto sibling = entries.get<Entries::Sibling>( childIndex );
				while ( sibling != *find )
				{
					childIndex = sibling;
					sibling = entries.get<Entries::Sibling>( childIndex );
				}

				entries.get<Entries::Sibling>( childIndex ) = entries.get<Entries::Sibling>( sibling );
			}
		}
	}

	void TransformManager::SetPosition( Entity entity, const Vector& position )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return;
		else
		{
			entries.get<Entries::Position>( *find ) = ToXMFLOAT3( position );
			entries.get<Entries::Dirty>( *find ) = true;
		}
	}
	Vector TransformManager::GetPosition( Entity entity )const noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return Vector();
		else
			return ToVector( entries.peek<Entries::Position>( *find ) );
	}
	void TransformManager::SetRotation( Entity entity, const Vector& rotation )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return;
		else
		{

			auto quat = XMQuaternionRotationRollPitchYawFromVector( XMLoadFloat3( ( XMFLOAT3* )&rotation ) );
			XMStoreFloat4( &entries.get<Entries::Rotation>( *find ), quat );
			entries.get<Entries::Dirty>( *find ) = true;
		}
	}
	Vector TransformManager::GetRotation( Entity entity ) const noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return Vector();
		else
		{
			//XMQuaternionToAxisAngle
			//	return ToVector(entries.peek<Entries::Rotation>(*find));
			return Vector();
		}

	}
	void TransformManager::SetScale( Entity entity, const Vector& scale )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return;
		else
		{
			entries.get<Entries::Scale>( *find ) = ToXMFLOAT3( scale );
			entries.get<Entries::Dirty>( *find ) = true;
		}
	}
	Vector TransformManager::GetScale( Entity entity ) const noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return Vector();
		else
			return ToVector( entries.peek<Entries::Scale>( *find ) );
	}
	void TransformManager::SetTransform( Entity entity, const Matrix& transform )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return;
		else
		{
			XMMATRIX trans = XMLoadFloat4x4( ( XMFLOAT4X4* )&transform );
			XMVECTOR scale;
			XMVECTOR pos;
			XMVECTOR rot;
			XMMatrixDecompose( &scale, &rot, &pos, trans );
			XMStoreFloat3( &entries.get<Entries::Scale>( *find ), scale );
			XMStoreFloat3( &entries.get<Entries::Position>( *find ), pos );
			XMStoreFloat4( &entries.get<Entries::Rotation>( *find ), rot );
			entries.get<Entries::Dirty>( *find ) = true;
		}

	}
	Matrix TransformManager::GetTransform( Entity entity )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return Matrix();
		else
		{
			Matrix mat;
			XMStoreFloat4x4( ( XMFLOAT4X4* )&mat, XMLoadFloat4x4( &entries.peek<Entries::Transform>( *find ) ) );
			return mat;
		}

	}

	void TransformManager::RegisterTransformUser( Manager_TransformUser* tUser )noexcept
	{
		PROFILE;
		for ( auto tu : transformUsers )
			if ( tu == tUser )
				return;

		transformUsers.push_back( tUser );
	}

	void TransformManager::UnregisterTransformUser( Manager_TransformUser* tUser )noexcept
	{
		PROFILE;
		for ( auto& tu : transformUsers )
			if ( tu == tUser )
			{
				tu = transformUsers[transformUsers.size() - 1];
				transformUsers.pop_back();
				break;
			}
	}



	void TransformManager::Destroy( Entity entity )noexcept
	{
		PROFILE;
		if ( auto find = entries.find( entity ); !find.has_value() )
			return;
		else
			entries.erase( *find );
	}
	void TransformManager::DestroyEntities( const Entity entities[], uint32_t numEntities )noexcept
	{
		PROFILE;
		for ( uint32_t i = 0; i < numEntities; i++ )
		{
			if ( auto find = entries.find( entities[i] ); !find.has_value() )
				return;
			else
				entries.erase( *find );
		}
	}
	uint64_t TransformManager::get_memory_usage() const noexcept
	{
		return entries.get_memory_usage() + sizeof( *this );
	}
	void TransformManager::shrink_to_fit()noexcept
	{
		PROFILE;
		entries.shrink_to_fit();
	}

	uint32_t TransformManager::GetNumberOfRegisteredEntities() const noexcept
	{
		return static_cast< uint32_t >( entries.size() );
	}
	void TransformManager::GetRegisteredEntities( Entity entities[], uint32_t numEntities ) const noexcept
	{
		if ( numEntities != entries.size() )
			return;
		memcpy( entities, entries.peek<Entries::Entity>(), sizeof( Entity ) * numEntities );
	}

	void TransformManager::Frame()noexcept
	{
		PROFILE;
		GarbageCollection();
		UpdateDirtyEntities();
	}

	uint64_t TransformManager::GetDataWriter( Entity entity, std::function<bool( std::ostream& stream )>& writer )const noexcept
	{

		if ( auto findF = entries.find( entity ); !findF.has_value() )
			return 0;


		writer = [this, entity]( std::ostream& stream )
		{
			if ( auto find = entries.find( entity ); !find.has_value() )
				return false;
			else
			{
				Utilities::Binary_Stream::write( stream, version );
				Utilities::Binary_Stream::write( stream, entries.peek<Entries::Position>( *find ) );
				Utilities::Binary_Stream::write( stream, entries.peek<Entries::Rotation>( *find ) );
				Utilities::Binary_Stream::write( stream, entries.peek<Entries::Scale>( *find ) );
				return true;
			}
		};

		return sizeof( XMFLOAT3 ) * 2 + sizeof( XMFLOAT4 ) + sizeof( version );
	}

	bool TransformManager::is_registered( Entity entity ) const noexcept
	{
		return entries.find( entity ).has_value();
	}

	void TransformManager::CreateFromResource( Entity entity, Resources::Resource resource )
	{
		struct Component {
			decltype( version ) ver;
			XMFLOAT3 pos;
			XMFLOAT4 rot;
			XMFLOAT3 scale;
		};

		PROFILE;
		if ( auto find = entries.find( entity ); find.has_value() )
			return;

		if ( !initInfo.entityManager->IsAlive( entity ) )
			return;

		{
			auto index = entries.add( entity );

			resource.use_data( [this, index]( const Utilities::Memory::ConstMemoryBlock data )
			{
				entries.get<Entries::Position>( index ) = data.peek<Component>().pos;
				entries.get<Entries::Rotation>( index ) = data.peek<Component>().rot;
				entries.get<Entries::Scale>( index ) = data.peek<Component>().scale;

			} );
			
			
			XMStoreFloat4x4( &entries.get<Entries::Transform>( index ), XMMatrixIdentity() );
			entries.get<Entries::Dirty>( index ) = true;
			entries.get<Entries::Parent>( index ) = -1;
			entries.get<Entries::Child>( index ) = -1;
			entries.get<Entries::Sibling>( index ) = -1;
			entries.get<Entries::Flags>( index ) = TransformFlags::NONE;
		}
	}

	Utilities::GUID TransformManager::GetManagerType() const noexcept
	{
		return "Transform";
	}

	void TransformManager::DestroyAll()noexcept
	{
		PROFILE;
		entries.clear();
	}

	void TransformManager::ToggleActive( Entity entity, bool active )noexcept
	{}

	void TransformManager::ToggleActive( const Entity entities[], uint32_t numEntites, bool active )noexcept
	{}

	void TransformManager::write_to_stream( std::ostream& stream ) const
	{
		PROFILE;
		Utilities::Binary_Stream::read( stream, version );
		entries.writeToFile( stream );
	}
	void TransformManager::create_from_stream( std::istream& file )
	{
		PROFILE;
		uint32_t ver = 0;
		file.read( ( char* )&ver, sizeof( ver ) );
		entries.readFromFile( file );
		auto diry = entries.get<Entries::Dirty>();
		auto transforms = entries.get<Entries::Transform>();
		for ( uint32_t i = 0; i < entries.size(); i++ )
		{
			diry[i] = true;
			XMStoreFloat4x4( &transforms[i], XMMatrixIdentity() );
		}
	}
	void TransformManager::UpdateDirtyEntities()noexcept
	{
		PROFILE;
		auto transforms = entries.peek<Entries::Transform>();
		std::vector<uint32_t> entitiesToApplyParentTransformTo;

		for ( size_t i = 0; i < entries.size(); i++ )
		{
			if ( entries.get<Entries::Dirty>( i ) )
			{
				const auto& translation = XMMatrixTranslationFromVector( XMLoadFloat3( &entries.get<Entries::Position>( i ) ) );
				//const auto& qrot = XMMatrixRotationQuaternion()
				const auto& rotation = XMMatrixRotationQuaternion( XMLoadFloat4( &entries.get<Entries::Rotation>( i ) ) );
				const auto& scale = XMMatrixScalingFromVector( XMLoadFloat3( &entries.get<Entries::Scale>( i ) ) );
				XMStoreFloat4x4( &transforms[i], scale * rotation * translation );
				//XMMatrixTransformation()
				if ( entries.get<Entries::Parent>( i ) == -1 )
				{
					auto child = entries.get<Entries::Child>( i );
					while ( child != -1 )
					{
						entitiesToApplyParentTransformTo.push_back( child );
						child = entries.get<Entries::Sibling>( child );
					}
				}
			}
		}

		do
		{
			auto entitiesToApplyTransTo = std::move( entitiesToApplyParentTransformTo );
			for ( auto i : entitiesToApplyTransTo )
			{
				const auto& parentTransform = XMLoadFloat4x4( &transforms[entries.get<Entries::Parent>( i )] );
				const auto& childTransform = XMLoadFloat4x4( &transforms[i] );

				XMStoreFloat4x4( ( XMFLOAT4X4* )&transforms[i], childTransform * parentTransform );
				auto child = entries.get<Entries::Child>( i );
				while ( child != -1 )
				{
					entitiesToApplyParentTransformTo.push_back( child );
					child = entries.get<Entries::Sibling>( child );
				}
			}
		} while ( entitiesToApplyParentTransformTo.size() );

		auto entities = entries.get<Entries::Entity>();
		for ( auto tu : transformUsers )
			tu->UpdateEntityTransforms( ( Matrix* )transforms, entities, static_cast< uint32_t >( entries.size() ) );
	}
	void TransformManager::GarbageCollection()noexcept
	{
		PROFILE;
		uint32_t alive_in_row = 0;
		const uint32_t quitWhenReached = std::max( uint32_t( entries.size() * 0.1f ), 40U );
		while ( entries.size() > 0 && alive_in_row < quitWhenReached )
		{
			std::uniform_int_distribution<size_t> distribution( 0U, entries.size() - 1U );
			size_t i = distribution( generator );
			if ( initInfo.entityManager->IsAlive( entries.get<Entries::Entity>( i ) ) )
			{
				alive_in_row++;
				continue;
			}
			alive_in_row = 0;
			if ( entries.get<Entries::Parent>( i ) != -1 )
			{
				UnbindParent( entries.get<Entries::Entity>( i ), TransformFlags::NONE );
			}
			entries.erase( i );
		}
	}
}
