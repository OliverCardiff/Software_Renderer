#include "stdafx.h"
#include "Rasterizer.h"


using namespace TakeOne;

void Rasterizer::DrawFlat(HDC& hdc, Camera& cam)
{
	_CurrentRender = "Flat Shading";
	SpinLights();
	_currentVP = cam.GetViewPort();
	_tempG->Clear(Gdiplus::Color::Black);
	
	list<DrawInfo*>::iterator i = _info.begin();

	for(;i != _info.end(); i++)
	{
		ProcessVectorsFlat((*(*i)), cam);
		ProcessPolys((*(*i)), cam);
	}

	DepthSortPolys();

	list<Polygn*>::iterator j = _validPolys.begin();
	
	int p = 0;
	for(;j != _validPolys.end(); j++)
	{
		p++;
		DrawPolyFlat((*j));
		_onScrPolys++;
	}
	
	_validPolys.clear();
}

void Rasterizer::DrawPolyFlat(Polygn* p)
{
	SetLeftTopRight(p);

	_points[0].X = _vtop->_vec._x;
	_points[0].Y = _vtop->_vec._y;

	_points[1].X = _vright->_vec._x;
	_points[1].Y = _vright->_vec._y;

	_points[2].X = _vleft->_vec._x;
	_points[2].Y = _vleft->_vec._y;

	_nrm = p->_indices[0]->_normal + p->_indices[1]->_normal + p->_indices[0]->_normal;
	_nrm.MakeUnit();

	BYTE br,bb,bg;
	float r,g,b;

	r = (p->_indices[0]->_r + p->_indices[1]->_r + p->_indices[2]->_r)/3;
	b = (p->_indices[0]->_b + p->_indices[1]->_b + p->_indices[2]->_b)/3;
	g = (p->_indices[0]->_g + p->_indices[1]->_g + p->_indices[2]->_g)/3;

	br = (BYTE)((r * (255 - AMBIENT)) + AMBIENT);
	bb = (BYTE)((b * (255 - AMBIENT)) + AMBIENT);
	bg = (BYTE)((g * (255 - AMBIENT)) + AMBIENT);

	Gdiplus::SolidBrush brush(Gdiplus::Color(br, bg, bb));

	_tempG->FillPolygon(&brush, &_points[0], 3);

}
void Rasterizer::ProcessVectorsFlat(DrawInfo& info, Camera& cam)
{
	Matrix temp = cam.GetAll() * *info.worldMatrix;
	Vector worldVec;

	Vector diff, view, reflection;
	float dist, strength, dotProd,r,b,g;
	dist = strength = dotProd = r = g = b = 0;

	for(int i = 0; i < info.numVerts; i++)
	{
		worldVec = *info.worldMatrix * info.vert[i]._vec;
		info.transVerts[i]._vec = temp * info.vert[i]._vec;
		info.transVerts[i].SaveZ(cam.GetDepth());
		info.transVerts[i].Dehomogenize();
		info.transVerts[i]._normal = *info.rotationOnly * info.vert[i]._normal;
		info.transVerts[i]._normal.MakeUnit();
		info.transVerts[i]._light = max(0,info.transVerts[i]._normal.DotProduct(_lightDir));
		
		r = b = g = 0;
		
		_onScrVerts++;
		for(int j = 0;j < 3; j++)
		{
			diff = _lights[j].position - worldVec;
			dist = diff.Magnitude();
			diff.MakeUnit();

			strength = max(0, 1 - ((dist*dist) / (_lights[j].LIGHT_LENGTH * _lights[j].LIGHT_LENGTH)));
			dotProd = max(0, info.transVerts[j]._normal.DotProduct(diff));

			r += _lights[j]._r * strength * dotProd;
			b += _lights[j]._b * strength * dotProd;
			g += _lights[j]._g * strength * dotProd;
		}
		
		info.transVerts[i]._r = min(r,1);
		info.transVerts[i]._b = min(b,1);
		info.transVerts[i]._g = min(g,1);

		if(info.transVerts[i]._light != 0)
		{
		//calculate specularity
			view = cam.GetView() * info.vert[i]._vec;
			view.MakeUnit();

			reflection = info.transVerts[i]._normal;
			reflection.Multiply(2 * info.transVerts[i]._light);
			reflection.Subtract(_lightDir);
			reflection.MakeUnit();
			
			dp = max(0, pow(max(0,view.DotProduct(reflection)), 5.0f));
			info.transVerts[i]._spec = min(1, dp);
		}
		else
		{
			info.transVerts[i]._spec = 0;
		}
	}

	
}


void Rasterizer::DrawWireFrame(HDC& hdc, Camera& cam)
{
	_CurrentRender = "Wireframe mode";
	SpinLights();
	_currentVP = cam.GetViewPort();
	_tempG->Clear(Gdiplus::Color::Black);
	
	list<DrawInfo*>::iterator i = _info.begin();

	for(;i != _info.end(); i++)
	{
		ProcessVectorsFlat((*(*i)), cam);
		ProcessPolys((*(*i)), cam);
	}

	DepthSortPolys();

	list<Polygn*>::iterator j = _validPolys.begin();
	
	int p = 0;
	for(;j != _validPolys.end(); j++)
	{
		p++;
		DrawPolyWire((*j));
		_onScrPolys++;
	}
	
	_validPolys.clear();
}

void Rasterizer::DrawPolyWire(Polygn* p)
{
	SetLeftTopRight(p);

	_points[0].X = _vtop->_vec._x;
	_points[0].Y = _vtop->_vec._y;

	_points[1].X = _vright->_vec._x;
	_points[1].Y = _vright->_vec._y;

	_points[2].X = _vleft->_vec._x;
	_points[2].Y = _vleft->_vec._y;

	_nrm = p->_indices[0]->_normal + p->_indices[1]->_normal + p->_indices[0]->_normal;
	_nrm.MakeUnit();

	BYTE br,bb,bg;
	float r,g,b;

	r = (p->_indices[0]->_r + p->_indices[1]->_r + p->_indices[2]->_r)/3;
	b = (p->_indices[0]->_b + p->_indices[1]->_b + p->_indices[2]->_b)/3;
	g = (p->_indices[0]->_g + p->_indices[1]->_g + p->_indices[2]->_g)/3;

	br = (BYTE)((r * (255 - AMBIENT)) + AMBIENT);
	bb = (BYTE)((b * (255 - AMBIENT)) + AMBIENT);
	bg = (BYTE)((g * (255 - AMBIENT)) + AMBIENT);

	Gdiplus::Pen pen(Gdiplus::Color(br, bg, bb));

	_tempG->DrawLine(&pen, _points[0], _points[1]);
	_tempG->DrawLine(&pen, _points[1], _points[2]);
	_tempG->DrawLine(&pen, _points[0], _points[2]);

}