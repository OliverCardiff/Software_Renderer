#include "StdAfx.h"
#include "Rasterizer.h"

using namespace TakeOne;

void Rasterizer::DrawFence(HDC& hdc, Camera& cam)
{
	_CurrentRender = "Fence Shading";
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
			ScanPolyFenceTex((*j));
		}
		else
		{
			ScanPolyFence((*j));
		}
		_onScrPolys++;
	}
	
	_validPolys.clear();

	_bitmap->UnlockBits(_bmd);
}


//--- Textured Fence ---//
int Rasterizer::TraceEdgeFenceTex(Vertex *one, Vertex *two, EndPoint *arr, Polygn *p, int offset)
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

				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
				_fspec = min(1, dp);

				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].uv.x = _u;
					arr[index].uv.y = _v;
					arr[index].spec = _fspec;
					arr[index].light = _flight;
					index++;
				}
				else
				{
					_u %= p->_bmpX;
					_v %= p->_bmpY;
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

				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
				_fspec = min(1, dp);

				arr[index].y = y;
				arr[index].x = x;
				arr[index].uv.x = _u;
				arr[index].uv.y = _v;
				arr[index].spec = _fspec;
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
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x* (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));

				Vector::Lerp(_nrm, one->_normal, two->_normal, tmp);
				Vector::Lerp(_eye, one->_eye, two->_eye, tmp);

				_nrm.MakeUnit();
				_eye.MakeUnit();

				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
				_fspec = min(1, dp);


				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].uv.x = _u;
					arr[index].uv.y = _v;
					arr[index].spec = _fspec;
					arr[index].light = _flight;
					index++;
				}
				else
				{
					_u %= p->_bmpX;
					_v %= p->_bmpY;
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

				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
				_fspec = min(1, dp);

				arr[index].y = y;
				arr[index].x = x;
				arr[index].uv.x = _u;
				arr[index].uv.y = _v;
				arr[index].spec = _fspec;
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

void Rasterizer::ScanPolyFenceTex(Polygn *p)
{
	SetLeftTopRight(p);
	
	startCount = TraceEdgeFenceTex(_vtop, _vleft, _st, p, 0);
	endCount = TraceEdgeFenceTex(_vtop, _vright, _ed, p, 0);

	if(startCount == endCount && endCount == 0) return;
	
	if(startCount > endCount)
	{
		if(endCount != 0) endCount--;
		endCount = TraceEdgeFenceTex(_vright, _vleft, _ed, p, endCount);
	}
	else 
	{
		if(startCount != 0) startCount--;
		startCount = TraceEdgeFenceTex(_vleft, _vright, _st, p, startCount);
	}

	int loopControl = min(endCount, startCount);

	for(int i = 0;i < loopControl; i++)
	{
		ScanLineTex(&_st[i], &_ed[i], p);
	}
}




//--- Untextured Fence ---//
int Rasterizer::TraceEdgeFence(Vertex *one, Vertex *two, EndPoint *arr, Polygn *p, int offset)
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

				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
				_fspec = min(1, dp);

				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].spec = _fspec;
					arr[index].light = _flight;
					index++;
				}
				else
				{
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

				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
				_fspec = min(1, dp);

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index].spec = _fspec;
				arr[index].light = _flight;
				index++;

			}

			error -= deltaX;
			if(error < 0)
			{
				x += xStep;
				error += deltaY;
			}
			if(index > 200)
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
				
				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
				_fspec = min(1, dp);

				if(x != lastY)
				{
					lastY = x;
					arr[index].y = x;
					arr[index].x = y;
					arr[index].spec = _fspec;
					arr[index].light = _flight;
					index++;
				}
				else
				{
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

				_flight = max(0,_nrm.DotProduct(_lightDir));
		
				_reflection = _nrm - _lightDir;
				_reflection.Multiply(2 * _flight);
				_reflection.MakeUnit();
				
				dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), PHONG_EXP));
				_fspec = min(1, dp);

				lastY = x;
				arr[index].y = y;
				arr[index].x = x;
				arr[index].spec = _fspec;
				arr[index].light = _flight;
				index++;
			}

			error -= deltaX;
			if(error < 0)
			{
				x += xStep;  
				error -= deltaY;
			}
			if(index > 200)
			{
				break;
			}
		}
	}

	return index;
}

void Rasterizer::ScanPolyFence(Polygn *p)
{
	SetLeftTopRight(p);
	
	startCount = TraceEdgeFence(_vtop, _vleft, _st, p, 0);
	endCount = TraceEdgeFence(_vtop, _vright, _ed, p, 0);

	if(startCount == endCount && endCount == 0) return;
	
	if(startCount > endCount)
	{
		if(endCount != 0) endCount--;
		endCount = TraceEdgeFence(_vright, _vleft, _ed, p, endCount);
	}
	else 
	{
		if(startCount != 0) startCount--;
		startCount = TraceEdgeFence(_vleft, _vright, _st, p, startCount);
	}

	int loopControl = min(endCount, startCount);

	for(int i = 0;i < loopControl; i++)
	{
		ScanLineFence(&_st[i], &_ed[i], p);
	}
}

void Rasterizer::ScanLineFence(EndPoint *one, EndPoint *two, Polygn *p)
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

					_flight = one->light * _subDx + two->light * (1 - _subDx);
					_fspec = one->spec * _subDx + two->spec * (1 - _subDx);

					InterpolDrawNoCheck(p, x0, y0);
				}
			}
			else
			{
				for(;x0 <= x1; x0++)
				{
					_subDx = ((float)(x1 - x0)) * _dx;
					
					_flight = two->light * _subDx + one->light * (1 - _subDx);
					_fspec = two->spec * _subDx + one->spec * (1 - _subDx);

					InterpolDrawNoCheck(p, x0, y0);
				}
			}
		}
	}
}
