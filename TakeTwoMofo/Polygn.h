#pragma once
#include "Vector.h"
#include "Vertex.h"

namespace TakeOne
{
	class Polygn
	{
	public:
		Polygn(void);
		~Polygn(void);
	public:
		bool operator <(const Polygn& rhs);
		bool operator >(const Polygn& rhs);
		static bool Compare(Polygn * one, Polygn * two);
		Vertex* _indices[3];
		Gdiplus::BitmapData* _bmp;
		Gdiplus::BitmapData* _bump;
		int _bmpX;
		int _bmpY;
		Vector MakeNormal();
		bool wireFrame;
		void AvgZ();
		float _avgZ;
		bool textured;
		bool bumpMapped;
	};
}
