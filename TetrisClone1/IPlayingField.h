#pragma once
#include <stdint.h>
#include <vector>

#define STANDARD_WIDTH 12
#define STANDARD_HEIGHT 18
#define PLAY_AREA STANDARD_WIDTH*STANDARD_HEIGHT

class IPlayingField
{
public:
	using field = std::vector<uint32_t>;
	struct Buffer {
		field field;
		uint32_t clearedLines;

		Buffer& operator=(const Buffer& that) {
			this->clearedLines = that.clearedLines;
			this->field = that.field; 
			return *this; 
		}
	};

	
	virtual ~IPlayingField() {};
	virtual void Draw() = 0; 
	virtual void UpdateDisplayBuffer(Buffer*) = 0;
	virtual void UpdateScore(uint32_t) = 0; 
	virtual uint32_t GetHeight() = 0; 
	virtual uint32_t GetWidth() = 0; 
};

