#include "pch.h"
#include "Tetris.h"
#include "Gameplay.h"
#include "MainMenu.h"

/*-----------------------------------------------------------------------------------------------*/
Tetris::Tetris(IPlayerInput& input, IPlayingField& display, timeTickDelayFunc delay) :
	playerInput(input),
	display(display),
	delay(delay)
{
	timeTicks = 1; 

	states.emplace(State::GAMEPLAY, std::make_shared<Gameplay>(display.GetWidth(), display.GetHeight()));
	states.emplace(State::MAIN_MENU, std::make_shared<MainMenu>());

	currentState = states[State::GAMEPLAY]; 

	for (int y = 0; y < display.GetHeight(); y++) { 
		for (int x = 0; x < display.GetWidth(); x++) { 
			outputBuffer.field.push_back(' ');
		} 
	} 

	outputBuffer.clearedLines = 0; 

} 

/*-----------------------------------------------------------------------------------------------*/
Tetris::~Tetris() 
{
	
}

/*-----------------------------------------------------------------------------------------------*/
void Tetris::Run()
{
	while (true)
	{
		updateTime();
		updateInputs();
		updateState();
		updateDisplay();
	}
}

/*-----------------------------------------------------------------------------------------------*/
void Tetris::updateTime() {
	delay(); 
	timeTicks++;
}

/*-----------------------------------------------------------------------------------------------*/
void Tetris::updateInputs()
{
	keys = playerInput.GetPlayerInputs();
}

/*-----------------------------------------------------------------------------------------------*/
void Tetris::updateState()
{	
	if (currentState->isDone) {
		currentState->Teardown();
		display.Reset(); 
		currentState->Setup(); 
	}
	else {
		currentState->Update(outputBuffer, keys, timeTicks);
	}
}

/*-----------------------------------------------------------------------------------------------*/
void Tetris::updateDisplay()
{
	display.UpdateDisplayBuffer(outputBuffer);
	display.Draw();

}