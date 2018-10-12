#include "StdAfx.h"
#include "Rasterizer.h"

using namespace TakeOne;


Rasterizer::Rasterizer(Camera * cam)
{
	_bitmap = new Gdiplus::Bitmap((int)cam->GetViewPort()._x, (int)cam->GetViewPort()._y, PixelFormat32bppARGB);
	_tempG = new Gdiplus::Graphics(_bitmap);
	_bmd = new Gdiplus::BitmapData();
	
	_cam = cam;
	_lightDir._x =  20;
	_lightDir._y =  7;
	_lightDir._z =  20;
	_lightDir.MakeUnit();
	MakePointLights();

	_onScrPolys = _onScrVerts = 0;
}

void Rasterizer::MakePointLights()
{
	_lights[0].position._y = 100;
	_lights[0].position._x = 0;
	_lights[0].position._z = 0;
	_lights[0].LIGHT_LENGTH = 500;
	_lights[0]._r = 0.8;
	_lights[0]._b = 0.3;
	_lights[0]._g = 0.1;
	_lights[1].position._z = 100;
	_lights[1].position._x = 0;
	_lights[1].position._y = 0;
	_lights[1].LIGHT_LENGTH = 500;
	_lights[1]._r = 0;
	_lights[1]._b = 0.5;
	_lights[1]._g = 1;
	_lights[2].position._x = 100;
	_lights[2].position._y = 0;
	_lights[2].position._z = 0;
	_lights[2].LIGHT_LENGTH = 500;
	_lights[2]._r = 1;
	_lights[2]._b = 0;
	_lights[2]._g = 0.5;
}
void Rasterizer::MutateLights()
{
	_lights[0].position._y = 800;
	_lights[0].position._x = 800;
	_lights[0].position._z = 800;
	_lights[0].LIGHT_LENGTH = 2000;

	_lights[1].position._y = 800;
	_lights[1].position._x = 800;
	_lights[1].position._z = 800;
	_lights[1].LIGHT_LENGTH = 2000;

	_lights[2].position._y = 800;
	_lights[2].position._x = 800;
	_lights[2].position._z = 800;
	_lights[2].LIGHT_LENGTH = 2000;
}
void Rasterizer::SpinLights()
{
	Matrix temp;

	Matrix::MakeRotation(X, ROT_AMOUNT *0.04f, temp, true);
	Matrix::Transform(temp, _lights[0].position);

	Matrix::MakeRotation(Y, ROT_AMOUNT * 0.05f, temp, true);
	Matrix::Transform(temp, _lights[1].position);

	Matrix::MakeRotation(Z, ROT_AMOUNT * 0.08f, temp, true);
	Matrix::Transform(temp, _lights[2].position);

}

Rasterizer::~Rasterizer(void)
{
	delete _bitmap;
	delete _tempG;
}
void Rasterizer::ClearInfo()
{
	_onScrPolys = _onScrVerts = 0;
	_info.clear();
}
void Rasterizer::Draw(HDC& hdc, Camera& cam)
{
	_CurrentRender = "Gourad Shading";
	SpinLights();
	_currentVP = cam.GetViewPort();
	_tempG->Clear(Gdiplus::Color::Black);
	Gdiplus::Rect r(0,0, (int)cam.GetViewPort()._x, (int)cam.GetViewPort()._y);
	
	_bitmap->LockBits(&r, Gdiplus::ImageLockModeRead, _bitmap->GetPixelFormat(), _bmd);
	
	_row = (BYTE  *)_bmd->Scan0;
	list<DrawInfo*>::iterator i = _info.begin();

	for(;i != _info.end(); i++)
	{
		if((*i)->textured == true)
		{
			ProcessVectors((*(*i)), cam);
		}
		else
		{
			ProcessVectorsFlat((*(*i)), cam);
		}
		ProcessPolys((*(*i)), cam);
	}

	DepthSortPolys();

	list<Polygn*>::iterator j = _validPolys.begin();
	
	int p = 0;
	for(;j != _validPolys.end(); j++)
	{
		p++;
		if((*j)->textured)
		{
			ScanPolyTex((*j));
		}
		else
		{
			ScanPoly((*j));
		}
		_onScrPolys++;
	}
	
	_validPolys.clear();

	_bitmap->UnlockBits(_bmd);
}

void Rasterizer::SubmitToDraw(DrawInfo& info)
{
	_info.push_back(&info);
}
void Rasterizer::ProcessVectors(DrawInfo& info, Camera& cam)
{
	Matrix temp = cam.GetAll() * *info.worldMatrix;
	Vector reflection;
	Vector view;
	float dp;

	for(int i = 0; i < info.numVerts; i++)
	{
		_onScrVerts++;

		info.transVerts[i]._vec = temp * info.vert[i]._vec;
		info.transVerts[i].SaveZ(cam.GetDepth());
		info.transVerts[i].Dehomogenize();
		info.transVerts[i]._normal = *info.rotationOnly * info.vert[i]._normal;
		info.transVerts[i]._normal.MakeUnit();
		info.transVerts[i]._light = max(0,info.transVerts[i]._normal.DotProduct(_lightDir));
		
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
//If a poly is inside the z - space, add to list
void Rasterizer::ProcessPolys(DrawInfo& info, Camera& cam)
{
	Vector2 v2 = cam.GetClipZ();;

	for(int i = 0;i < info.numPolys; i++)
	{
		info.polys[i].AvgZ();
		if(info.polys[i]._avgZ > v2._x && info.polys[i]._avgZ < v2._y &&
			info.polys[i].MakeNormal()._z < 0)
		{
			if(OnScreen(&info.polys[i]))
			{
				_validPolys.push_back(&info.polys[i]);
			}
		}
	}
}
void Rasterizer::DepthSortPolys()
{
	_validPolys.sort(Polygn::Compare);
}


void Rasterizer::ScanPoly(Polygn* p)
{
	SetLeftTopRight(p);
	
	startCount = TraceEdge(_vtop, _vleft,_st, p, 0, false);
	endCount = TraceEdge(_vtop, _vright, _ed, p, 0, true);
	if(startCount == endCount && endCount == 0) return;
	
	if(startCount > endCount)
	{
		if(endCount != 0) endCount--;
		endCount = TraceEdge(_vright, _vleft, _ed, p, endCount, true);
	}
	else 
	{
		if(startCount != 0) startCount--;
		startCount = TraceEdge(_vleft, _vright,  _st, p, startCount, false);
	}

	int loopControl = min(endCount, startCount);

	for(int i = 0;i < loopControl; i++)
	{
		ScanLine(&_st[i], &_ed[i], p);
	}
}
void Rasterizer::ScanLine(EndPoint* one, EndPoint* two, Polygn * p)
{
	x0 = one->x;
	x1 = two->x;
	y0 = one->y;

	if(y0 < _currentVP._y && y0 >= 0)
	{
		bool swap = false;

		if(x0 > x1) 
		{
			swap = true;
			int t = x0;
			x0 = x1;
			x1 = t;
		}
		
		if(x0 < _currentVP._x && x1 > 0)
		{
			
			if(x0 < 0) x0 = 0;
			if(x1 >= _currentVP._x) x1 = (int)_currentVP._x -1;

			_dx = (float)(x1 - x0);
			if(_dx != 0) _dx = 1/_dx;

			if(!swap)
			{
				for(;x0 <= x1; x0++)
				{
					_subDx = ((float)(x1 - x0)) * _dx;
					_r = (one->_r * _subDx) + (two->_r * (1 - _subDx));
					_b = (one->_b * _subDx) + (two->_b * (1 - _subDx));
					_g = (one->_g * _subDx) + (two->_g * (1 - _subDx));
					RGBDrawNoCheck(p, x0, y0);
				}
			}
			else
			{
				for(;x0 <= x1; x0++)
				{
					_subDx = ((float)(x1 - x0)) * _dx;
					_r = (two->_r * _subDx) + (one->_r * (1 - _subDx));
					_b = (two->_b * _subDx) + (one->_b * (1 - _subDx));
					_g = (two->_g * _subDx) + (one->_g * (1 - _subDx));
					RGBDrawNoCheck(p, x0, y0);
				}
			}
		}
	}
}

int Rasterizer::TraceEdge(Vertex* one, Vertex* two, EndPoint * arr, Polygn* p, int offset, bool endLine)
{
	x0 = (int)one->_vec._x;
	x1 = (int)two->_vec._x;
	y0 = (int)one->_vec._y;
	y1 = (int)two->_vec._y;

	int lastY = -1;
	int index = offset;
	bool swap = false;
	bool steep = abs(x1 - x0) > abs(y1 - y0);

	if(steep)
	{
		Swap(y0, x0);
		Swap(y1, x1);
	}
	if(y0 > y1)
	{
		swap = true;
	}

	deltaY = y1 - y0;
	deltaX = abs(x1 - x0);
	error = deltaY / 2;
	xStep;
	int x = x0;
	if(x0 < x1) xStep = 1; else xStep = -1;

	_dy = (float)y1 - y0;
	float tmp;

	if(!swap)
	{
		for(int y = y0;y <= y1; y++)
		{
			if(steep)
			{
				tmp = (y1 - y)/_dy;
				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_r = (one->_r * tmp) + (two->_r * (1 - tmp));
				_b = (one->_b * tmp) + (two->_b * (1 - tmp));
				_g = (one->_g * tmp) + (two->_g * (1 - tmp));

				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index]._r = _r;
					arr[index]._b = _b;
					arr[index]._g = _g;
					arr[index].light = _flight;
					index++;
				}
				else
				{
					RGBDraw(p,y,x);
				}				
			}
			else
			{
				tmp = (y1 - y)/_dy;
				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_r = (one->_r * tmp) + (two->_r * (1 - tmp));
				_b = (one->_b * tmp) + (two->_b * (1 - tmp));
				_g = (one->_g * tmp) + (two->_g * (1 - tmp));

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index]._r = _r;
				arr[index]._b = _b;
				arr[index]._g = _g;
				arr[index].light = _flight;
				index++;

			}

			error -= deltaX;
			if(error < 0)
			{
				x += xStep;
				error += deltaY;
			}
			if(index > MAX_POLY_SIZE )
			{
				break;
			}
		}
	}
	else
	{

		for(int y = y0;y >= y1; y--)
		{
			if(steep)
			{
				tmp = (y1 - y)/_dy;
				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_r = (one->_r * tmp) + (two->_r * (1 - tmp));
				_b = (one->_b * tmp) + (two->_b * (1 - tmp));
				_g = (one->_g * tmp) + (two->_g * (1 - tmp));

				if(x != lastY)
				{
					
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index]._r = _r;
					arr[index]._b = _b;
					arr[index]._g = _g;
					arr[index].light = _flight;
					index++;
				}
				else
				{
					RGBDraw(p,y,x);
				}
				
			}
			else
			{
				tmp = (y1 - y)/_dy;
				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_r = (one->_r * tmp) + (two->_r * (1 - tmp));
				_b = (one->_b * tmp) + (two->_b * (1 - tmp));
				_g = (one->_g * tmp) + (two->_g * (1 - tmp));

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index]._r = _r;
				arr[index]._b = _b;
				arr[index]._g = _g;
				arr[index].light = _flight;
				index++;
			}

			error -= deltaX;
			if(error < 0)
			{
				x += xStep;  
				error -= deltaY;
			}
			if(index > MAX_POLY_SIZE )
			{
				break;
			}
		}
	}

	return index;
}

bool Rasterizer::OnScreen(Polygn * p)
{
	if((p->_indices[0]->_vec._x < 0 && p->_indices[1]->_vec._x < 0 && p->_indices[2]->_vec._x < 0) ||
		(p->_indices[0]->_vec._x > _currentVP._x - 1 && p->_indices[1]->_vec._x > _currentVP._x - 1 && p->_indices[2]->_vec._x > _currentVP._x - 1) ||
		(p->_indices[0]->_vec._y < 0 && p->_indices[1]->_vec._y < 0 && p->_indices[2]->_vec._y < 0) ||
		(p->_indices[0]->_vec._y > _currentVP._y - 1 && p->_indices[1]->_vec._y > _currentVP._y - 1 && p->_indices[2]->_vec._y > _currentVP._y - 1))
	{
		return false;
	}
	return true;
}
void Rasterizer::InterpolDraw(Polygn* p, int x, int y)
{
	if(x > _currentVP._x - 1 || x < 0)
		return;
	if(y > _currentVP._y - 1 || y < 0)
		return;

	_light = (BYTE)((_flight + _fspec) * (255 - AMBIENT)) + AMBIENT;

	_temp = (BYTE*)_bmd->Scan0 + (y*_bmd->Stride);

	_temp[x * 4] = _light;
	_temp[x * 4 + 1] = _light;
	_temp[x * 4 + 2] = _light;
}
void Rasterizer::InterpolDrawNoCheck(Polygn* p, int x, int y)
{
	_light = (BYTE)min(255,((_fspec + _flight) * (255 - AMBIENT)) + AMBIENT);
	
	_temp = (BYTE*)_bmd->Scan0 + (y*_bmd->Stride);

	_temp[x * 4] = _light;
	_temp[x * 4 + 1] = _light;
	_temp[x * 4 + 2] = _light;
}
void Rasterizer::RGBDraw(Polygn* p, int x, int y)
{
	if(x > _currentVP._x - 1 || x < 0)
		return;
	if(y > _currentVP._y - 1 || y < 0)
		return;

	_br = (BYTE)((_r * (255 - AMBIENT)) + AMBIENT);
	_bb = (BYTE)((_b * (255 - AMBIENT)) + AMBIENT);
	_bg = (BYTE)((_g * (255 - AMBIENT)) + AMBIENT);

	_temp = (BYTE*)_bmd->Scan0 + (y*_bmd->Stride);

	_temp[x * 4] = _bb;
	_temp[x * 4 + 1] = _bg;
	_temp[x * 4 + 2] = _br;
}
void Rasterizer::RGBDrawNoCheck(Polygn* p, int x, int y)
{
	_br = (BYTE)((_r * (255 - AMBIENT)) + AMBIENT);
	_bb = (BYTE)((_b * (255 - AMBIENT)) + AMBIENT);
	_bg = (BYTE)((_g * (255 - AMBIENT)) + AMBIENT);
	
	_temp = (BYTE*)_bmd->Scan0 + (y*_bmd->Stride);

	_temp[x * 4] = _bb;
	_temp[x * 4 + 1] = _bg;
	_temp[x * 4 + 2] = _br;
}
float Rasterizer::Distance(int x, int y, float x1, float y1)
{
	_dx = x - x1;
	_dy = y - y1;

	return _dx * _dx + _dy * _dy;
}

void Rasterizer::Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}
bool Rasterizer::IsInsideX(int x)
{
	if(x > -1 && x < _cam->GetViewPort()._x)
		return true;
	return false;
}
bool Rasterizer::IsInsideY(int x)
{
	if(x > -1 && x < _cam->GetViewPort()._y)
		return true;
	return false;
}
void Rasterizer::SetLeftTopRight(Polygn*p)
{
	if(p->_indices[0]->_vec._y < p->_indices[1]->_vec._y)
	{
		_vtop = p->_indices[0];
		_vtop->_vec._w = p->_indices[0]->_light;

		_vleft = p->_indices[1];
		_vleft->_vec._w = p->_indices[1]->_light;

		_vright = p->_indices[2];
		_vright->_vec._w = p->_indices[2]->_light;
	}
	else
	{
		_vtop = p->_indices[1];
		_vtop->_vec._w = p->_indices[1]->_light;

		_vleft = p->_indices[0];
		_vleft->_vec._w = p->_indices[0]->_light;

		_vright = p->_indices[2];
		_vright->_vec._w = p->_indices[2]->_light;
	}

	if(p->_indices[2]->_vec._y <_vtop->_vec._y) 
	{
		_vtop = p->_indices[2];
		_vtop->_vec._w = p->_indices[2]->_light;

		_vleft = p->_indices[1];
		_vleft->_vec._w = p->_indices[1]->_light;

		_vright = p->_indices[0];
		_vright->_vec._w = p->_indices[0]->_light;
	}
	if(_vleft->_vec._x > _vright->_vec._x)
	{
		Vertex * temp = _vright;
		_vright = _vleft;
		_vleft = temp;
	}
}

