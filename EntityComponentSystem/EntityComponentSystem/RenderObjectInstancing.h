
#pragma once
#include <Graphics/Renderer_Interface.h>
#include <Entity.h>
#include <Graphics/RenderJob.h>

namespace ECS
{
	class RenderObjectInstancing{
	public:
		RenderObjectInstancing( std::shared_ptr<Renderer::Renderer_Interface> renderer );
		void AddEntity( const Entity entity, Renderer::RenderJob& job, Renderer::RenderGroup = Renderer::RenderGroup::RENDER_PASS_0 );
		void RemoveEntity( const Entity entity );

	private:
		std::shared_ptr<Renderer::Renderer_Interface> renderer;
	};
}
