#pragma once
#include "IState.hpp"

class MainMenu : public IState
{
public:
	MainMenu();
	~MainMenu();

	void Setup() override;
	void Teardown() override;
	void Update(IPlayingField::buffer&, IPlayerInput::inputs, currentTime) override;

private:

};