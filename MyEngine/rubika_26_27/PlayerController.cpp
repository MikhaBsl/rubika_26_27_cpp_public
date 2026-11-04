#include <PlayerController.h>
#include <Globals.h>

PlayerController::PlayerController(TransformComponent* transform) : Transform(transform)
{
}

PlayerController::~PlayerController()
{
	if (gData.InputMgr == nullptr)
		return;

	gData.InputMgr->UnregisterFromAction("Up", this);
	gData.InputMgr->UnregisterFromAction("Down", this);
	gData.InputMgr->UnregisterFromAction("Left", this);
	gData.InputMgr->UnregisterFromAction("Right", this);
}

void PlayerController::Init()
{
	gData.InputMgr->CreateAction("Up");
	gData.InputMgr->CreateAction("Down");
	gData.InputMgr->CreateAction("Left");
	gData.InputMgr->CreateAction("Right");

	gData.InputMgr->SetActionKey("Up", sf::Keyboard::Key::Z);
	gData.InputMgr->SetActionKey("Down", sf::Keyboard::Key::S);
	gData.InputMgr->SetActionKey("Left", sf::Keyboard::Key::Q);
	gData.InputMgr->SetActionKey("Right", sf::Keyboard::Key::D);

	gData.InputMgr->RegisterToAction("Up", "Player Up", [this](float v) { OnUp(v); }, this);
	gData.InputMgr->RegisterToAction("Down", "Player Down", [this](float v) { OnDown(v); }, this);
	gData.InputMgr->RegisterToAction("Left", "Player Left", [this](float v) { OnLeft(v); }, this);
	gData.InputMgr->RegisterToAction("Right", "Player Right", [this](float v) { OnRight(v); }, this);
}

void PlayerController::Update(float deltaTime)
{
	if (Transform == nullptr)
		return;

	sf::Vector2f direction = MoveDir;
	if (direction.x != 0.f || direction.y != 0.f)
		direction = direction.normalized();

	Transform->SetWorldPosition(Transform->GetWorldPosition() + direction * Speed * deltaTime);
}

void PlayerController::OnUp(float value) { MoveDir.y = -value; }
void PlayerController::OnDown(float value) { MoveDir.y = value; }
void PlayerController::OnLeft(float value) { MoveDir.x = -value; }
void PlayerController::OnRight(float value) { MoveDir.x = value; }