#pragma once

namespace TakeOne
{
	class PointLight 
	{
	public:
		PointLight(void);
		~PointLight(void);
		float _r;
		float _g;
		float _b;
		Vector position;
	public:
		float LIGHT_LENGTH;
	};
}
