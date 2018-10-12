#pragma once
#include "Terrain.h"
#include "PhysObj.h"

namespace TakeOne
{
	class PhysicsManager
	{
	public:
		PhysicsManager(void);
		~PhysicsManager(void);
	public:
		void TerrainCollide(Vector& pos, Vector& vel);
		void Submit(PhysObj* p, bool mover);
		void Update(float time);
		Terrain * GetTerrain();

	private:
		void InsideBox(float farX, float nearX, float farZ, float nearZ, Vector& pos, Vector& vel);
		float Distance(int x, int y, float x1, float y1);
		float GetHeightAt(float scPosX, float scPosZ);
		Terrain _terrain;
		std::list<PhysObj*> _movers;
		std::list<PhysObj*> _static;
	};
}