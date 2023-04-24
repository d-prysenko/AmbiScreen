#include <vector>
#include "../ScreenManager.h"

struct Serializable
{
	virtual std::vector<uint8_t> serialize() = 0;
};