#pragma once

namespace TakeOne
{
	class Vertex
	{
	public:
		Vertex(Vector& pos);
		Vertex(void);
		~Vertex(void);
	public:
		Vector _vec;
		Vector _eye;
		Vector _normal;
		POINT _uvCoords;
		float _spec;
		float _light;
		float _savedZ;
		void SaveZ(float oneOverDepth);
		void Dehomogenize();

		float _r;
		float _g;
		float _b;
	};
}
