#pragma once
#include "Polygn.h"
#include "Vertex.h"
#include "WorldObj.h"


namespace TakeOne
{
	class Bone: public WorldObj
	{
	public:
		Bone();
		Bone(int polys, int verts);
		~Bone(void);

	public:
		void BuildArrays(int polys, int verts);
		void RefreshNormals();
		void Draw(Rasterizer * _r);
		void Update(float time);
		void TranslatePos();
		void RotateXPos();
		
		Vertex * _verts;
		Vertex * _transVerts;
		Polygn * _polys;
		float _length;
		int _numPolys;
		int _numVerts;

	public:
		DrawInfo _info;
	};
}
