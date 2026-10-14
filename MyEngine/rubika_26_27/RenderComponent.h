#pragma once
#include <IDrawable.h>

class RenderComponent : IDrawable
{
public:
	RenderComponent();
	~RenderComponent();

	void Start() override;
	void Update(float fDeltaTime) override;
	void Destroy(float deltaTime) override;
};