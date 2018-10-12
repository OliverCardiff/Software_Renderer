#include "StdAfx.h"
#include "Terrain.h"

using namespace TakeOne;

Terrain::Terrain(void)
{
	MakeMap();
}

Terrain::~Terrain(void)
{
}

void Terrain::MakeMap()
{
	PerlinField();
	SmoothField();

	_texture = new Gdiplus::Bitmap(L"grass.bmp");
	Gdiplus::Bitmap *tempTex = new Gdiplus::Bitmap(512,512,PixelFormat32bppARGB);
	Gdiplus::Graphics * g = new Gdiplus::Graphics(tempTex);

	g->DrawImage(_texture, 0,0);
	delete _texture;
	delete g;
	_texture = tempTex;

	_bumpTex = new Gdiplus::Bitmap(L"woodywindow_NRM.bmp");
	Gdiplus::Bitmap *tempTex2 = new Gdiplus::Bitmap(512,512,PixelFormat32bppARGB);
	g = new Gdiplus::Graphics(tempTex2);

	g->DrawImage(_bumpTex, 0,0);
	delete _bumpTex;
	delete g;
	_bumpTex = tempTex2;

	Gdiplus::Rect r(0,0, _texture->GetHeight(), _texture->GetWidth());
	
	_data = new Gdiplus::BitmapData();
	_bumpData = new Gdiplus::BitmapData();
	
	_texture->LockBits(&r, Gdiplus::ImageLockMode::ImageLockModeRead, _texture->GetPixelFormat(), _data);
	_bumpTex->LockBits(&r, Gdiplus::ImageLockMode::ImageLockModeRead, _texture->GetPixelFormat(), _bumpData);

	int polys = (FIELD_X - 1) * (FIELD_X - 1) * 2;
	int verts = FIELD_X * FIELD_X;

	_map = new Bone(polys, verts);
	int stretchU = 0;
	int stretchV = 0;

	for(int i = 0;i < FIELD_X; i++)
	{
		for(int j = 0;j < FIELD_X; j++)
		{
			_map->_verts[i + (j * FIELD_X)]._vec = _map->_transVerts[i + (j * FIELD_X)]._vec = _field[j][i];
			
			stretchU = ((int)(i * ((float)BOX_SIZE))) % (int)(_texture->GetWidth() * 2) ;
			stretchV = ((int)(j * ((float)BOX_SIZE))) % (int)(_texture->GetHeight() * 2) ;

			_map->_verts[i + (j * FIELD_X)]._uvCoords.x = _map->_transVerts[i + (j * FIELD_X)]._uvCoords.x  
					= ((int)(i * ((float)BOX_SIZE*2)));

			_map->_verts[i + (j * FIELD_X)]._uvCoords.y = _map->_transVerts[i + (j * FIELD_X)]._uvCoords.y  
					=  ((int)(j * ((float)BOX_SIZE*2)));
		}
	}	

	int rowCounter = 1;
	bool skip = false;
	int pIndex = 0;

	for(int k = 0;k < verts - FIELD_X; k++)
	{
		if(rowCounter == FIELD_X)
		{rowCounter = 0; skip = true;}
		else skip = false;

		if(!skip)
		{
			_map->_polys[pIndex]._indices[0] = &_map->_transVerts[k];
			_map->_polys[pIndex]._indices[1] = &_map->_transVerts[k + 1];
			_map->_polys[pIndex]._indices[2] = &_map->_transVerts[k + FIELD_X];
			_map->_polys[pIndex]._bmp = _data;
			_map->_polys[pIndex].textured = true;
			_map->_polys[pIndex]._bmpX = (int)_texture->GetWidth();
			_map->_polys[pIndex]._bmpY = (int)_texture->GetHeight();
			_map->_polys[pIndex]._bump = _bumpData;
			_map->_polys[pIndex].bumpMapped = true;
			pIndex++;
			
			_map->_polys[pIndex]._indices[0] = &_map->_transVerts[k + FIELD_X];
			_map->_polys[pIndex]._indices[1] = &_map->_transVerts[k + 1];
			_map->_polys[pIndex]._indices[2] = &_map->_transVerts[k + FIELD_X + 1];
			_map->_polys[pIndex]._bmp = _data;
			_map->_polys[pIndex]._bmpX = (int)_texture->GetWidth();
			_map->_polys[pIndex]._bmpY = (int)_texture->GetHeight();
			_map->_polys[pIndex].textured = true;
			_map->_polys[pIndex]._bump = _bumpData;
			_map->_polys[pIndex].bumpMapped = true;
			pIndex++;
		}
		
		rowCounter++;
	}

	_map->RefreshNormals();

	_bones.push_back(_map);

}

void Terrain::PerlinField()
{
	bool newRow = true;
	bool newColumn = true;

	float segment = FIELD_X /4;
	float power;
	int rowCounter = 0, colCounter = 0;

	while(segment >= 1)
	{
		rowCounter = (int)segment; colCounter = (int)segment;
		power = segment/(FIELD_X/2);

		for(int i = 0;i < FIELD_X; i++)
		{
			if(rowCounter == segment)
			{newRow = true; rowCounter = 0;}
			else newRow = false;

			for(int j = 0;j < FIELD_X; j++)
			{
				if(colCounter == segment)
				{newColumn = true; colCounter = 0;}
				else newColumn = false;

				_field[j][i]._x = (float)(BOX_SIZE * j);
				_field[j][i]._z = (float)(BOX_SIZE * i);
				

				if(newColumn)
				{
					if(newRow)
					{
						_field[j][i]._y += GetURand() * STEEPNESS * power;
					}
					else
					{
						_field[j][i]._y = _field[j][i-1]._y;
					}
				}
				else
				{
					_field[j][i]._y = _field[j-1][i]._y;
				}
				colCounter++;
			}
			rowCounter++;
		}
		segment /= 2;
	}
}

void Terrain::SmoothField()
{
	int arr[FIELD_X][FIELD_X];

	for(int i = 0;i < FIELD_X; i++)
	{
		for(int j = 0;j < FIELD_X; j++)
		{
			arr[i][j] = 0;

			_field[i][j]._w = _field[i][j]._y;
			arr[i][j]++;

			if(!OutsideArray(i-1, j-1))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i - 1][j - 1]._y;
			}
			if(!OutsideArray(i-1, j))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i - 1][j]._y;
			}
			if(!OutsideArray(i-1, j+1))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i - 1][j + 1]._y;
			}
			if(!OutsideArray(i, j-1))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i][j - 1]._y;
			}
			if(!OutsideArray(i, j+1))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i][j + 1]._y;
			}
			if(!OutsideArray(i+1, j-1))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i + 1][j - 1]._y;
			}
			if(!OutsideArray(i+1, j))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i + 1][j]._y;
			}
			if(!OutsideArray(i+1, j+1))
			{
				arr[i][j]++;

				_field[i][j]._w += _field[i + 1][j + 1]._y;
			}
		}
	}

	for(int i = 0;i < FIELD_X; i++)
	{
		for(int j = 0;j < FIELD_X; j++)
		{
			_field[i][j]._y = _field[i][j]._w / arr[i][j];
			_field[i][j]._w = 1;
		}
	}
}

bool Terrain::OutsideArray(int i, int j)
{
	if(i < 0 || i >= FIELD_X || j < 0 || j >= FIELD_X)
		return true;
	else return false;
}