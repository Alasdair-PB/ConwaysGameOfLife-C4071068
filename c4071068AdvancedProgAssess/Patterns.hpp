#include "Pattern.hpp"
#pragma once
#include <iostream>
using namespace std;
namespace Patterns
{

	template <size_t N>
	PatternMask<N> rotate90(const PatternMask<N>& pattern) {
		std::bitset<N> rotatedPattern;
		Vector2<int> originalSize = pattern.dimensions;
		Vector2<int> newSize(originalSize.y, originalSize.x); 

		for (int i = 0; i < originalSize.y; ++i) {
			for (int j = 0; j < originalSize.x; ++j) {
				int newI = j;
				int newJ = originalSize.y - 1 - i;
				if (pattern.pattern[i * originalSize.x + j]) {
					rotatedPattern.set(newI * newSize.x + newJ);
				}
			}
		}
		return PatternMask<N>(rotatedPattern, newSize);
	}

	enum class FlipAxis { Horizontal, Vertical };

	template <size_t N>
	PatternMask<N> flip(const PatternMask<N>& pattern, FlipAxis axis) {
		std::bitset<N> flippedPattern;
		Vector2<int> originalSize = pattern.dimensions;

		for (int i = 0; i < originalSize.y; ++i) {
			for (int j = 0; j < originalSize.x; ++j) {
				int newI = i;
				int newJ = j;

				if (axis == FlipAxis::Horizontal) 
					newJ = originalSize.x - 1 - j;  
				else if (axis == FlipAxis::Vertical) 
					newI = originalSize.y - 1 - i;  

				if (pattern.pattern[i * originalSize.x + j]) 
					flippedPattern.set(newI * originalSize.x + newJ);
			}
		}
		return PatternMask<N>(flippedPattern, originalSize);
	}


	// *************************************** Still Lifes *****************************************************************

	const PatternMask<16> block(std::bitset<16>("0000011001100000"), Vector2<int>(4, 4));
	const PatternMask<30> beehive(std::bitset<30>("000000001100010010001100000000"), Vector2<int>(6, 5)); 

	// *************************************** Blinker *****************************************************************

	const PatternMask<15> blinkerA(std::bitset<15>("000010010010000"), Vector2<int>(3, 5));
	const PatternMask<15> blinkerB = rotate90(blinkerA);

	// *************************************** Toads *****************************************************************

	const PatternMask<24> toadA1(std::bitset<24>("000000001110011100000000"), Vector2<int>(6, 4));  
	const PatternMask<24> toadA2= rotate90(toadA1); 
	const PatternMask<24> toadA3 = flip(toadA1, FlipAxis::Horizontal);
	const PatternMask<24> toadA4 = flip(toadA2, FlipAxis::Vertical);

	const PatternMask<36> toadB1(std::bitset<36>("000000000100010010010010001000000000"), Vector2<int>(6, 6)); 
	const PatternMask<36> toadB2 = rotate90(toadB1);
	const PatternMask<36> toadB3 = flip(toadB1, FlipAxis::Horizontal);
	const PatternMask<36> toadB4 = flip(toadB2, FlipAxis::Vertical);

	// *************************************** Gliders *****************************************************************

	const PatternMask<25> gliderA1(std::bitset<25>("0000001000001100110000000"), Vector2<int>(5, 5));
	const PatternMask<25> gliderA2 = rotate90(gliderA1);
	const PatternMask<25> gliderA3 = flip(gliderA1, FlipAxis::Horizontal);
	const PatternMask<25> gliderA4 = flip(gliderA2, FlipAxis::Vertical);

	const PatternMask<25> gliderB1(std::bitset<25>("0000000010010100011000000"), Vector2<int>(5, 5));
	const PatternMask<25> gliderB2 = rotate90(gliderB1);
	const PatternMask<25> gliderB3 = flip(gliderB1, FlipAxis::Horizontal);
	const PatternMask<25> gliderB4 = flip(gliderB2, FlipAxis::Vertical);

	const PatternMask<25> gliderC1 = flip(gliderA2, FlipAxis::Horizontal);
	const PatternMask<25> gliderC2 = rotate90(gliderC1);
	const PatternMask<25> gliderC3 = flip(gliderC1, FlipAxis::Horizontal);
	const PatternMask<25> gliderC4 = flip(gliderC2, FlipAxis::Vertical);

	const PatternMask<25> gliderD1 = flip(gliderB2, FlipAxis::Horizontal);
	const PatternMask<25> gliderD2 = rotate90(gliderD1);
	const PatternMask<25> gliderD3 = flip(gliderD1, FlipAxis::Horizontal);
	const PatternMask<25> gliderD4 = flip(gliderD2, FlipAxis::Vertical);

	// *************************************** LWSS *****************************************************************

	const PatternMask<42> lwssA1(std::bitset<42>("000000000111100100010000001001001000000000"), Vector2<int>(7, 6));

	const PatternMask<42> lwssA2 = rotate90(lwssA1);
	const PatternMask<42> lwssA3 = flip(lwssA1, FlipAxis::Horizontal);
	const PatternMask<42> lwssA4 = flip(lwssA2, FlipAxis::Vertical);

	const PatternMask<42> lwssB1(std::bitset<42>("000000000011000110110011110000110000000000"), Vector2<int>(7, 6));
	const PatternMask<42> lwssB2 = rotate90(lwssB1);
	const PatternMask<42> lwssB3 = flip(lwssB1, FlipAxis::Horizontal);
	const PatternMask<42> lwssB4 = flip(lwssB2, FlipAxis::Vertical);

	const PatternMask<42> lwssC1 = flip(lwssA1, FlipAxis::Vertical);
	const PatternMask<42> lwssC2 = rotate90(lwssC1);
	const PatternMask<42> lwssC3 = flip(lwssC1, FlipAxis::Horizontal);
	const PatternMask<42> lwssC4 = flip(lwssC2, FlipAxis::Vertical);

	const PatternMask<42> lwssD1 = flip(lwssB1, FlipAxis::Vertical);
	const PatternMask<42> lwssD2 = rotate90(lwssD1);
	const PatternMask<42> lwssD3 = flip(lwssD1, FlipAxis::Horizontal);
	const PatternMask<42> lwssD4 = flip(lwssD2, FlipAxis::Vertical);
}