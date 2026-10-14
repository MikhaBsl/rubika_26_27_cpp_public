#include <IComponent.h>

IComponent::IComponent(Entity& entity) : ParentComponent(entity)
{

}

IComponent::~IComponent()
{
}
