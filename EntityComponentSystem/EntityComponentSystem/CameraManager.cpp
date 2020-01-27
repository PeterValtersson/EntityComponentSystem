#include "CameraManager.h"
#include <Utilities/Profiler/Profiler.h>

ECS::CameraManager::CameraManager( const CameraManager_Init_Info& ii ) : init_info( ii )
{
	if ( !ii.entity_manager )
		throw CouldNotCreateManager( "Camera manager must have an entity manager" );

	if ( !ii.transform_manager )
		throw CouldNotCreateManager( "Camera manager must have an transform manager" );
}

ECS::CameraManager::~CameraManager() noexcept
{}

void ECS::CameraManager::Create( const Entity& entity, const Camera_Create_Info& info ) noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); find.has_value() )
		return;

	if ( !init_info.entity_manager->IsAlive( entity ) )
		return;

	auto index = entries.add( entity );
	entries.set<Entries::Dirty>( index, ~0 );
	entries.set<Entries::FoV>( index, info.fov );
	entries.set<Entries::AspectRatio>( index, info.aspectRatio );
	entries.set<Entries::NearPlane>( index, info.nearPlane );
	entries.set<Entries::FarPlane>( index, info.farPlane );
	DirectX::XMStoreFloat4x4( &entries.get<Entries::View>( index ), DirectX::XMMatrixIdentity() );

	init_info.transform_manager->Create( entity, info.position, info.rotation );
}

void ECS::CameraManager::UpdateCamera( const Entity& entity, const Camera_Create_Info& info ) noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); !find.has_value() )
		return;
	else
	{
		entries.set<Entries::Dirty>( *find, ~0 );
		entries.set<Entries::FoV>( *find, info.fov );
		entries.set<Entries::AspectRatio>( *find, info.aspectRatio );
		entries.set<Entries::NearPlane>( *find, info.nearPlane );
		entries.set<Entries::FarPlane>( *find, info.farPlane );
	}
}
const ECS::Matrix& to_ecs_mat( const DirectX::XMFLOAT4X4& m )
{
	return *( ECS::Matrix* ) & m;
}
const ECS::Vector& to_ecs_vec( const DirectX::XMFLOAT3& v )
{
	return *( ECS::Vector* ) & v;
}
ECS::Matrix ECS::CameraManager::GetView( const Entity& entity ) const noexcept
{
	PROFILE;
	ECS::Matrix mat;
	if ( auto find = entries.find( entity ); !find.has_value() )
		DirectX::XMStoreFloat4x4( ( DirectX::XMFLOAT4X4* ) & mat, DirectX::XMMatrixIdentity() );
	else
		mat = to_ecs_mat( entries.peek<Entries::View>( *find ) );
	return mat;
}

ECS::Matrix ECS::CameraManager::GetViewInv( const Entity& entity ) const noexcept
{
	PROFILE;
	ECS::Matrix mat;
	if ( auto find = entries.find( entity ); !find.has_value() )
		DirectX::XMStoreFloat4x4( ( DirectX::XMFLOAT4X4* ) & mat, DirectX::XMMatrixIdentity() );
	else
	{
		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4( &entries.peek<Entries::View>( *find ) );
		view = DirectX::XMMatrixInverse( nullptr, view );
		DirectX::XMStoreFloat4x4( ( DirectX::XMFLOAT4X4* ) & mat, view );
	}
	return mat;
}

ECS::Matrix ECS::CameraManager::GetProjection( const Entity& entity ) const noexcept
{
	PROFILE;
	ECS::Matrix mat;
	if ( auto find = entries.find( entity ); !find.has_value() )
		DirectX::XMStoreFloat4x4( ( DirectX::XMFLOAT4X4* ) & mat, DirectX::XMMatrixIdentity() );
	else
	{
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH( entries.peek<Entries::FoV>( *find ),
																	entries.peek<Entries::AspectRatio>( *find ),
																	entries.peek<Entries::NearPlane>( *find ),
																	entries.peek<Entries::FarPlane>( *find ) );
		DirectX::XMStoreFloat4x4( ( DirectX::XMFLOAT4X4* ) & mat, proj );
	}
	return mat;
}

ECS::Matrix ECS::CameraManager::GetViewProjection( const Entity& entity ) const noexcept
{
	PROFILE;
	ECS::Matrix mat;
	if ( auto find = entries.find( entity ); !find.has_value() )
		DirectX::XMStoreFloat4x4( ( DirectX::XMFLOAT4X4* ) & mat, DirectX::XMMatrixIdentity() );
	else
	{
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH( entries.peek<Entries::FoV>( *find ),
																	entries.peek<Entries::AspectRatio>( *find ),
																	entries.peek<Entries::NearPlane>( *find ),
																	entries.peek<Entries::FarPlane>( *find ) );
		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4( &entries.peek<Entries::View>( *find ) );
		DirectX::XMStoreFloat4x4( ( DirectX::XMFLOAT4X4* ) & mat, view * proj );
	}
	return mat;
}

ECS::Ray ECS::CameraManager::WorldSpaceRayFromScreenPos( const Entity& entity, int x, int y, int screenWidth, int screenHeight ) const noexcept
{
	PROFILE;
	if ( auto find = entries.find( entity ); !find.has_value() )
		return { {0, 0, 0}, {1, 0, 0} };
	else
	{
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH( entries.peek<Entries::FoV>( *find ),
																	entries.peek<Entries::AspectRatio>( *find ),
																	entries.peek<Entries::NearPlane>( *find ),
																	entries.peek<Entries::FarPlane>( *find ) );
		DirectX::XMFLOAT4X4 projF;
		//DirectX::XMStoreFloat4x4( &projF, proj );
		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4( &entries.peek<Entries::View>( *find ) );
		DirectX::XMMATRIX invView = DirectX::XMMatrixInverse( nullptr, view );

		float xNDC = ( ( 2.0f * x ) / screenWidth ) - 1;
		float yNDC = -( ( ( 2.0f * y ) / screenHeight ) - 1 );

		float xView = xNDC / proj.r[0].m128_f32[0]; //projF._11;
		float yView = yNDC / proj.r[1].m128_f32[1]; //projF._22;

		Ray ray;
		auto direction = DirectX::XMVector3Normalize( DirectX::XMVector4Transform( DirectX::XMVectorSet( xView, yView, 1.0f, 0.0f ),
																				   invView ) );
		DirectX::XMStoreFloat3( ( DirectX::XMFLOAT3* ) & ray.direction, direction );
		ray.origin = init_info.transform_manager->GetPosition( entity );
	}
}

bool ECS::CameraManager::is_registered( Entity entity ) const noexcept
{
	return entries.find( entity ).has_value();
}

void ECS::CameraManager::CreateFromResource( Entity entity, Resources::Resource resource )
{}

uint64_t ECS::CameraManager::GetDataWriter( Entity entity, std::function<bool( std::ostream & stream )>& writer ) const noexcept
{
	return uint64_t();
}

void ECS::CameraManager::Destroy( Entity entity ) noexcept
{
	PROFILE;
	entries.erase( entity );
}

void ECS::CameraManager::DestroyMultiple( const Entity entities[], size_t numEntities ) noexcept
{
	PROFILE;
	for ( size_t i = 0; i < numEntities; i++ )
		entries.erase( entities[i] );
}

void ECS::CameraManager::DestroyMultiple( const std::vector<Entity>& entities ) noexcept
{
	PROFILE;
	for ( auto e : entities )
		entries.erase( e );
}

void ECS::CameraManager::DestroyAll() noexcept
{
	PROFILE;
	entries.clear();
}

void ECS::CameraManager::ToggleActive( Entity entity, bool active ) noexcept
{}

void ECS::CameraManager::ToggleActive( const Entity entities[], size_t numEntities, bool active ) noexcept
{}

void ECS::CameraManager::ToggleActive( const std::vector<Entity>& entities, bool active ) noexcept
{}

size_t ECS::CameraManager::GetNumberOfRegisteredEntities() const noexcept
{
	return entries.size();
}

void ECS::CameraManager::GetRegisteredEntities( Entity entities[], size_t numEntities ) const noexcept
{
	memcpy( entities, entries.peek<Entries::Entity>(), std::min( numEntities, entries.size() ) * sizeof( Entity ) );
}

std::vector<ECS::Entity> ECS::CameraManager::GetRegisteredEntities() const noexcept
{
	return std::vector<Entity>( entries.peek<Entries::Entity>(), entries.peek<Entries::Entity>() + entries.size() );
}

void ECS::CameraManager::Frame() noexcept
{
	PROFILE;
	GarbageCollection();
	for(size_t i = 0; i < entries.size(); i++ )
		if ( entries.peek<Entries::Dirty>( i ) != ~0 )
		{
			DirectX::XMMATRIX transform = DirectX::XMLoadFloat4x4( &initInfo.transformManager->GetCleanedTransforms()[cameraData.dirty[currentActive.activeCamera]] );
			DirectX::XMVECTOR pos = XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f );// XMLoadFloat3(&transformManager->positions[tindex]);
															   //	auto rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transformManager->rotations[tindex]));
			DirectX::XMVECTOR forward = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
			DirectX::XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
			pos = XMVector3TransformCoord( pos, transform );
			forward = XMVector3TransformNormal( forward, transform );
			up = XMVector3TransformNormal( up, transform );
			XMVECTOR lookAt = pos + forward;

			XMMATRIX view = XMMatrixLookAtLH( pos, lookAt, up );
			XMStoreFloat4x4( &cameraData.view[currentActive.activeCamera], view );

		}
}

Utilities::GUID ECS::CameraManager::GetManagerType() const noexcept
{
	return Utilities::GUID();
}

size_t ECS::CameraManager::get_memory_usage() const noexcept
{
	return size_t();
}

void ECS::CameraManager::shrink_to_fit()
{}

void ECS::CameraManager::write_to_stream( std::ostream& stream ) const
{}

void ECS::CameraManager::read_from_stream( std::istream& stream )
{}

void ECS::CameraManager::GarbageCollection() noexcept
{
	PROFILE;
	uint32_t alive_in_row = 0;
	const uint32_t quitWhenReached = std::max( uint32_t( entries.size() * 0.1f ), 40U );
	while ( entries.size() > 0 && alive_in_row < quitWhenReached )
	{
		std::uniform_int_distribution<size_t> distribution( 0U, entries.size() - 1U );
		size_t i = distribution( generator );
		if ( init_info.entity_manager->IsAlive( entries.get<Entries::Entity>( i ) ) )
		{
			alive_in_row++;
			continue;
		}
		alive_in_row = 0;
		entries.erase( i );
	}
}
