#pragma once
#include "IState.hpp"

class MainMenu : public IState
{
public:
	MainMenu();
	~MainMenu();

	void Setup() override;
	void Teardown() override;
	void Update(IPlayingField::Buffer&, IPlayerInput::inputs, currentTime) override;

private:

};