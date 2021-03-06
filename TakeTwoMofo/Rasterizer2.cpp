#include "StdAfx.h"
#include "Rasterizer.h"

using namespace TakeOne;

void Rasterizer::ScanPolyTex(Polygn* p)
{
	SetLeftTopRight(p);
	
	startCount = TraceEdgeTex(_vtop, _vleft, _st, p, 0);
	endCount = TraceEdgeTex(_vtop, _vright, _ed, p, 0);
	if(startCount == endCount && endCount == 0) return;
	
	if(startCount > endCount)
	{
		if(endCount != 0) endCount--;
		endCount = TraceEdgeTex(_vright, _vleft, _ed, p, endCount);
	}
	else 
	{
		if(startCount != 0) startCount--;
		startCount = TraceEdgeTex(_vleft, _vright, _st, p, startCount);
	}

	int loopControl = min(endCount, startCount);

	for(int i = 0;i < loopControl; i++)
	{
		ScanLineTex(&_st[i], &_ed[i], p);
	}

	/*if(startCount > endCount)
	{
		for(int j = endCount;j < startCount; j++)
		{
			ScanLineTex(_st[j].y, _st[j].x , _st[j].x, _st[j].light, _st[j].light, p);
		}
	}
	else if(endCount > startCount)
	{
		for(int k = startCount;k < endCount; k++)
		{
			ScanLineTex(_ed[k].y, _ed[k].x , _ed[k].x , _ed[k].light, _ed[k].light, p);
		}	
	}*/
}
void Rasterizer::ScanLineTex(EndPoint * one, EndPoint * two, Polygn * p)
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
					_flight = two->light * _subDx + one->light * (1 - _subDx);
					_fspec = two->spec * _subDx + one->spec * (1 - _subDx);

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

int Rasterizer::TraceEdgeTex(Vertex* one, Vertex* two, EndPoint * arr, Polygn* p, int offset)
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
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x * (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));
				
				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_fspec = (one->_spec * tmp) + (two->_spec * (1-tmp));

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
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x * (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));
				
				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_fspec = (one->_spec * tmp) + (two->_spec * (1-tmp));

				lastY = x;
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
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x * (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));

				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_fspec = (one->_spec * tmp) + (two->_spec * (1-tmp));

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
				_u = (int)(((float)one->_uvCoords.x * tmp) + ((float)two->_uvCoords.x * (1 - tmp)));
				_v = (int)(((float)one->_uvCoords.y * tmp) + ((float)two->_uvCoords.y * (1 - tmp)));

				_flight = (one->_light * tmp) + (two->_light * (1 - tmp));
				_fspec = (one->_spec * tmp) + (two->_spec * (1-tmp));

				lastY = x;
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
void Rasterizer::InterpolDrawTex(Polygn* p, int x, int y)
{
	if(x > _currentVP._x - 1 || x < 0)
		return;
	if(y > _currentVP._y - 1 || y < 0)
		return;
	
	_lTemp = (BYTE*)p->_bmp->Scan0 + ( _v * p->_bmp->Stride);

	_flight = _flight * (1- TEX_AMBIENT) + TEX_AMBIENT;

	_temp = (BYTE*)_bmd->Scan0 + (y*_bmd->Stride);

	_temp[x * 4] = (BYTE)min(255, ((float)_lTemp[_u * 4] * _flight) + (_fspec * 255));
	_temp[x * 4 + 1] = (BYTE)min(255,((float)_lTemp[_u * 4 + 1] * _flight)+ (_fspec * 255));
	_temp[x * 4 + 2] = (BYTE)min(255,((float)_lTemp[_u * 4 + 2] * _flight)+ (_fspec * 255));
}
void Rasterizer::InterpolDrawNoCheckTex(Polygn* p, int x, int y)
{
	_lTemp = (BYTE*)p->_bmp->Scan0 + ( _v * p->_bmp->Stride);

	_flight = _flight * (1- TEX_AMBIENT) + TEX_AMBIENT;

	_temp = (BYTE*)_bmd->Scan0 + (y*_bmd->Stride);

	_temp[x * 4] = (BYTE)min(255, ((float)_lTemp[_u * 4] * _flight) + (_fspec * 255));
	_temp[x * 4 + 1] = (BYTE)min(255,((float)_lTemp[_u * 4 + 1] * _flight)+ (_fspec * 255));
	_temp[x * 4 + 2] = (BYTE)min(255,((float)_lTemp[_u * 4 + 2] * _flight)+ (_fspec * 255));

	/*_temp[x * 4] = (BYTE)((float)_lTemp[_u * 4] * _flight);
	_temp[x * 4 + 1] = (BYTE)((float)_lTemp[_u * 4 + 1] * _flight);
	_temp[x * 4 + 2] = (BYTE)((float)_lTemp[_u * 4 + 2] * _flight);*/

}