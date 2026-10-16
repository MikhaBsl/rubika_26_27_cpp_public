#pragma once

#include <string>

class Entity;

class IComponent abstract
{
public:
	IComponent() = delete;
	IComponent(Entity& entity);

	virtual ~IComponent();

	virtual const char* GetName() const = 0;

	virtual void DrawDebug() = 0;

	virtual void Start() = 0;

	virtual void Update(float fDeltaTime) = 0;

	virtual void Destroy() = 0;

protected:
	Entity& GetParentEntity() const { return ParentComponent; };

private:
	Entity& ParentComponent;
};
