#pragma once

class ITetromino
{
public:
	virtual ~ITetromino() {};
	virtual void Rotate() = 0;
};

