#include "RenderObjectInstancing.h"

ECS::RenderObjectInstancing::RenderObjectInstancing( std::shared_ptr<Renderer::Renderer_Interface> renderer ) : renderer( renderer )
{}

void ECS::RenderObjectInstancing::AddEntity( const Entity entity, Renderer::RenderJob& job, Renderer::RenderGroup )
{}

void ECS::RenderObjectInstancing::RemoveEntity( const Entity entity )
{}
