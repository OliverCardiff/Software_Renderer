#include "StdAfx.h"
#include "Rasterizer.h"

using namespace TakeOne;

void Rasterizer::ScanLinePhongBump(EndPoint *one, EndPoint *two, Polygn *p)
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
					_u = (int)(((float)one->uv.x * _subDx) + ((float)two->uv.x * (1 - _subDx)));
					_v = (int)(((float)one->uv.y * _subDx) + ((float)two->uv.y * (1 - _subDx)));
					
					_u %= p->_bmpX;
					_v %= p->_bmpY;


					Vector::Lerp(_nrm, one->normal, two->normal, _subDx);
					Vector::Lerp(_eye, one->eye, two->eye, _subDx);

					_nrm.MakeUnit();
					_eye.MakeUnit();

					_bump = (BYTE*)p->_bump->Scan0 + ( _v * p->_bump->Stride);
					_bumpVec._x = 128 - _bump[_u * 4  + 2];
					_bumpVec._y = 128 - _bump[_u * 4 + 1];
					_bumpVec._z =  _bump[_u * 4 ];
					_bumpVec.MakeUnit();

					_nrm = _nrm + (_bumpVec * BUMP_POWER);
					_nrm.MakeUnit();

					_flight = max(0,_nrm.DotProduct(_lightDir));
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
					_fspec = min(1,dp);
					
					
					InterpolDrawNoCheckTex(p, x0, y0);
				}
			}
			else
			{
				for(;x0 <= x1; x0++)
				{
					_subDx = ((float)(x1 - x0)) * _dx;

					
					_u = (int)(((float)two->uv.x * _subDx) + ((float)one->uv.x * (1 - _subDx)));
					_v = (int)(((float)two->uv.y * _subDx) + ((float)one->uv.y * (1 - _subDx)));

					_u %= p->_bmpX;
					_v %= p->_bmpY;
					
					Vector::Lerp(_nrm, two->normal, one->normal, _subDx);
					Vector::Lerp(_eye, two->eye, one->eye, _subDx);

					_nrm.MakeUnit();
					_eye.MakeUnit();

					_bump = (BYTE*)p->_bump->Scan0 + ( _v * p->_bump->Stride);
					_bumpVec._x = 128 - _bump[_u * 4 +2 ];
					_bumpVec._y = 128 - _bump[_u * 4 + 1];
					_bumpVec._z =  _bump[_u * 4 ];
					_bumpVec.MakeUnit();

					_nrm = _nrm + (_bumpVec * BUMP_POWER);
					_nrm.MakeUnit();

					_flight = max(0,_nrm.DotProduct(_lightDir));
					_reflection = _nrm - _lightDir;
					_reflection.Multiply(2 * _flight);
					_reflection.MakeUnit();
					dp = max(0, pow(max(0,_eye.DotProduct(_reflection)), TEX_PHONG_EXP));
					_fspec = min(1, dp);
					


					InterpolDrawNoCheckTex(p, x0, y0);
				}
			}
		}
	}
}