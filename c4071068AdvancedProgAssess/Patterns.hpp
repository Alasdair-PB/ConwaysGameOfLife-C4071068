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


	// Method for get Dimensions

	const PatternMask<16> block(std::bitset<16>("0000011001100000"), Vector2<int>(4, 4));
	const PatternMask<30> beehive(std::bitset<30>("000000001100010010001100000000"), Vector2<int>(6, 5)); 

	const PatternMask<15> blinkerA(std::bitset<15>("000010010010000"), Vector2<int>(3, 5));
	const PatternMask<15> blinkerB = rotate90(blinkerA);

	// Toad frame 1 all orientations
	const PatternMask<24> toadA1(std::bitset<24>("000000001110011100000000"), Vector2<int>(6, 4));  
	const PatternMask<24> toadA2= rotate90(toadA1); 
	const PatternMask<24> toadA3 = flip(toadA1, FlipAxis::Horizontal);
	const PatternMask<24> toadA4 = flip(toadA2, FlipAxis::Vertical);

	const PatternMask<36> toadB1(std::bitset<36>("000000000100010010010010001000000000"), Vector2<int>(6, 6)); 
	const PatternMask<36> toadB2 = rotate90(toadB1);
	const PatternMask<36> toadB3 = flip(toadB1, FlipAxis::Horizontal);
	const PatternMask<36> toadB4 = flip(toadB2, FlipAxis::Vertical);

	const PatternMask<25> glider(std::bitset<25>("0000000100000100111000000"), Vector2<int>(5, 5));
	const PatternMask<42> lwss(std::bitset<42>("000000000100100100000010001001111000000000"), Vector2<int>(6, 7));


	// a glider or a LWSS appears.
}