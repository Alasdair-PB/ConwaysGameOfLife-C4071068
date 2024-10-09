#pragma once


// Using Templates

template <typename T>
class Vector2
{
	public : 
		T x;
		T y;

		Vector2(T x, T y) 
		{
			this->x = x;
			this->y = y;
		}

		Vector2<T> operator +(T scalar) const 
		{
			return Vector2<T>(this->x + scalar, this->y + scalar);
		}
};