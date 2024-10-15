#include "Pattern.hpp"
#pragma once

namespace Patterns
{
	// Method for get Dimensions

	const PatternMask<16> block(std::bitset<16>("0000011001100000"), Vector2<int>(4, 4));
	const PatternMask<30> beehive(std::bitset<30>("000000001100010010001100000000"), Vector2<int>(6, 5)); 

	const PatternMask<15> blinkerA(std::bitset<15>("000010010010000"), Vector2<int>(3, 5));
	const PatternMask<15> blinkerB(std::bitset<15>("000000111000000"), Vector2<int>(5, 3));


	const PatternMask<24> toadA1(std::bitset<24>("000000001110011100000000"), Vector2<int>(6, 4));  
	const PatternMask<24> toadA2(std::bitset<24>("000000100110110010000000"), Vector2<int>(4, 6));  
	const PatternMask<24> toadA3(std::bitset<24>("000000011100111000000000"), Vector2<int>(6, 4));  
	const PatternMask<24> toadA4(std::bitset<24>("000000100110110010000000"), Vector2<int>(4, 6));  

	const PatternMask<24> toadB1(std::bitset<24>("000000011000000110000000"), Vector2<int>(6, 6));  

	const PatternMask<25> glider(std::bitset<25>("0000000100000100111000000"), Vector2<int>(5, 5));
	const PatternMask<42> lwss(std::bitset<42>("000000000100100100000010001001111000000000"), Vector2<int>(6, 7));


	// a glider or a LWSS appears.
}