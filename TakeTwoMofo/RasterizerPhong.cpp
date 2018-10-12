#include "StdAfx.h"
#include "Rasterizer.h"

using namespace TakeOne;

//--- Untextured Phong Stuff ---//
void Rasterizer::ScanLinePhong(EndPoint *one, EndPoint *two, Polygn *p)
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

					Vector::Lerp(_nrm, one->normal, two->normal, _subDx);
					Vector::Lerp(_eye, one->eye, two->eye, _subDx);

					_nrm.MakeUnit();
					_eye.MakeUnit();

					_flight = max(0,_nrm.DotProduct(_lightDir));
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
					_fspec = min(1, dp);

					InterpolDrawNoCheck(p, x0, y0);
				}
			}
			else
			{
				for(;x0 <= x1; x0++)
				{
					_subDx = ((float)(x1 - x0)) * _dx;
					
					Vector::Lerp(_nrm, two->normal, one->normal, _subDx);
					Vector::Lerp(_eye, two->eye, one->eye, _subDx);

					_nrm.MakeUnit();
					_eye.MakeUnit();

					_flight = max(0,_nrm.DotProduct(_lightDir));
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
					_fspec = min(1, dp);

					InterpolDrawNoCheck(p, x0, y0);
				}
			}
		}
	}
}


int Rasterizer::TraceEdgePhong(Vertex *one, Vertex *two, EndPoint *arr, Polygn *p, int offset)
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
	if( y0 > y1)
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

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].eye = _eye;
					arr[index].normal = _nrm;
					index++;
				}
				else
				{

					_flight = max(0,_nrm.DotProduct(_lightDir));
			
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
					_fspec = min(1, dp);

					InterpolDraw(p,y,x);
				}				
			}
			else
			{
				tmp = (y1 - y)/_dy;

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index].eye = _eye;
				arr[index].normal = _nrm;
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

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();


				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].eye = _eye;
					arr[index].normal = _nrm;
					index++;
				}
				else
				{
					_flight = max(0,_nrm.DotProduct(_lightDir));
			
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
					_fspec = min(1, dp);

					InterpolDraw(p,y,x);
				}
				
			}
			else
			{
				tmp = (y1 - y)/_dy;

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index].eye = _eye;
				arr[index].normal = _nrm;
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

void Rasterizer::ScanPolyPhong(Polygn *p)
{
	SetLeftTopRight(p);
	
	startCount = TraceEdgePhong(_vtop, _vleft, _st, p, 0);
	endCount = TraceEdgePhong(_vtop, _vright, _ed, p, 0);

	if(startCount == endCount && endCount == 0) return;
	
	if(startCount > endCount)
	{
		if(endCount != 0) endCount--;
		endCount = TraceEdgePhong(_vright, _vleft, _ed, p, endCount);
	}
	else 
	{
		if(startCount != 0) startCount--;
		startCount = TraceEdgePhong(_vleft, _vright, _st, p, startCount);
	}

	int loopControl = min(endCount, startCount);

	for(int i = 0;i < loopControl; i++)
	{
		ScanLinePhong(&_st[i], &_ed[i], p);
	}
}
//--- Textured Phong Stuff ---//
void Rasterizer::ScanLinePhongTex(EndPoint *one, EndPoint *two, Polygn *p)
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

					Vector::Lerp(_nrm, one->normal, two->normal, _subDx);
					Vector::Lerp(_eye, one->eye, two->eye, _subDx);

					_nrm.MakeUnit();
					_eye.MakeUnit();

					_flight = max(0,_nrm.DotProduct(_lightDir));
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
					_fspec = min(1,dp);
					

					_u = (int)(((float)one->uv.x * _subDx) + ((float)two->uv.x * (1 - _subDx)));
					_v = (int)(((float)one->uv.y * _subDx) + ((float)two->uv.y * (1 - _subDx)));
					
					_u %= p->_bmpX;
					_v %= p->_bmpY;
					InterpolDrawNoCheckTex(p, x0, y0);
				}
			}
			else
			{
				for(;x0 <= x1; x0++)
				{
					_subDx = ((float)(x1 - x0)) * _dx;
					
					Vector::Lerp(_nrm, two->normal, one->normal, _subDx);
					Vector::Lerp(_eye, two->eye, one->eye, _subDx);

					_nrm.MakeUnit();
					_eye.MakeUnit();

					_flight = max(0,_nrm.DotProduct(_lightDir));
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
					_fspec = min(1, dp);
					

					_u = (int)(((float)two->uv.x * _subDx) + ((float)one->uv.x * (1 - _subDx)));
					_v = (int)(((float)two->uv.y * _subDx) + ((float)one->uv.y * (1 - _subDx)));

					_u %= p->_bmpX;
					_v %= p->_bmpY;

					InterpolDrawNoCheckTex(p, x0, y0);
				}
			}
		}
	}
}

int Rasterizer::TraceEdgePhongTex(Vertex *one, Vertex *two, EndPoint *arr, Polygn *p, int offset)
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
	if( y0 > y1)
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
	if(_dy == 0) _dy = 1;

	float tmp;

	if(!swap)
	{
		for(int y = y0;y <= y1; y++)
		{
			if(steep)
			{
				tmp = (y1 - y)/_dy;
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x* (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].uv.x = _u;
					arr[index].uv.y = _v;
					arr[index].eye = _eye;
					arr[index].normal = _nrm;
					index++;
				}
				else
				{
					_u %= p->_bmpX;
					_v %= p->_bmpY;

					_flight = max(0,_nrm.DotProduct(_lightDir));
			
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
					_fspec = min(1, dp);

					InterpolDrawTex(p,y,x);
				}				
			}
			else
			{
				tmp = (y1 - y)/_dy;
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x* (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index].uv.x = _u;
				arr[index].uv.y = _v;
				arr[index].eye = _eye;
				arr[index].normal = _nrm;
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
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x* (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				if(x != lastY)
				{
					
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].uv.x = _u;
					arr[index].uv.y = _v;
					arr[index].eye = _eye;
					arr[index].normal = _nrm;
					index++;
				}
				else
				{
					_u %= p->_bmpX;
					_v %= p->_bmpY;
					_flight = max(0,_nrm.DotProduct(_lightDir));
			
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
					_fspec = min(1, dp);

					InterpolDrawTex(p,y,x);
				}
				
			}
			else
			{
				tmp = (y1 - y)/_dy;
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x* (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index].uv.x = _u;
				arr[index].uv.y = _v;
				arr[index].eye = _eye;
				arr[index].normal = _nrm;
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

void Rasterizer::ScanPolyPhongTex(Polygn *p)
{
	SetLeftTopRight(p);
	
	startCount = TraceEdgePhongTex(_vtop, _vleft, _st, p, 0);
	endCount = TraceEdgePhongTex(_vtop, _vright, _ed, p, 0);

	if(startCount == endCount && endCount == 0) return;
	
	if(startCount > endCount)
	{
		if(endCount != 0) endCount--;
		endCount = TraceEdgePhongTex(_vright, _vleft, _ed, p, endCount);
	}
	else 
	{
		if(startCount != 0) startCount--;
		startCount = TraceEdgePhongTex(_vleft, _vright, _st, p, startCount);
	}

	int loopControl = min(endCount, startCount);

	for(int i = 0;i < loopControl; i++)
	{
		ScanLinePhongTex(&_st[i], &_ed[i], p);
	}
}



//--- General Phong Stuff ---//
void Rasterizer::DrawPhong(HDC& hdc, Camera& cam)
{
	_CurrentRender = "Phong Shading";
	_currentVP = cam.GetViewPort();
	_tempG->Clear(Gdiplus::Color::Black);
	Gdiplus::Rect r(0,0, (int)cam.GetViewPort()._x, (int)cam.GetViewPort()._y);
	
	_bitmap->LockBits(&r, Gdiplus::ImageLockModeRead, _bitmap->GetPixelFormat(), _bmd);
	
	_row = (BYTE  *)_bmd->Scan0;
	list<DrawInfo*>::iterator i = _info.begin();

	for(;i != _info.end(); i++)
	{
		ProcessVectorsPhong((*(*i)), cam);
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
			ScanPolyPhongTex((*j));
		}
		else
		{
			ScanPolyPhong((*j));
		}
		_onScrPolys++;
	}
	
	_validPolys.clear();

	_bitmap->UnlockBits(_bmd);
}

void Rasterizer::ProcessVectorsPhong(DrawInfo &info, Camera& cam)
{
	Matrix temp = cam.GetAll() * *info.worldMatrix;

	for(int i = 0; i < info.numVerts; i++)
	{
		info.transVerts[i]._vec = temp * info.vert[i]._vec;
		info.transVerts[i].SaveZ(cam.GetDepth());
		info.transVerts[i].Dehomogenize();
		info.transVerts[i]._normal = *info.rotationOnly * info.vert[i]._normal;
		info.transVerts[i]._normal.MakeUnit();
		info.transVerts[i]._light = max(0,info.transVerts[i]._normal.DotProduct(_lightDir));

		info.transVerts[i]._eye = cam.GetView() * info.vert[i]._vec;
		info.transVerts[i]._eye.MakeUnit();

		_onScrVerts++;
	}
}
