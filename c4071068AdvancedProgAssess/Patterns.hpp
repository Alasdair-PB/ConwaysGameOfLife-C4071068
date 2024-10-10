#include "Pattern.hpp"

namespace Patterns
{
	// Method for get Dimensions

	const PatternMask<16> block(std::bitset<16>("0000011001100000"), Vector2<int>(4, 4));
	const PatternMask<30> beehive(std::bitset<30>("000000001100010010001100000000"), Vector2<int>(6, 5)); // should include flipped
	const PatternMask<15> blinkerA(std::bitset<15>("000010010010000"), Vector2<int>(3, 5));
	const PatternMask<24> toadA(std::bitset<24>("000000001110011100000000"), Vector2<int>(6, 4));


	// a glider or a LWSS appears.
}