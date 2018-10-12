#pragma once
#include "Model.h"
#include "Bone.h"

const int FIELD_X = 16;
const int BOX_SIZE = 50;
const float STEEPNESS = 400;

namespace TakeOne
{
	class Terrain :
		public Model
	{
	public:
		Terrain(void);
		~Terrain(void);
	public:

		void MakeMap();
		void PerlinField();
		void SmoothField();
		bool OutsideArray(int i, int j);
		Vector _field[FIELD_X][FIELD_X];
		Gdiplus::Bitmap * _texture;
		Gdiplus::Bitmap * _bumpTex;
		Gdiplus::BitmapData * _data;
		Gdiplus::BitmapData * _bumpData;
		Bone * _map;
	};
}