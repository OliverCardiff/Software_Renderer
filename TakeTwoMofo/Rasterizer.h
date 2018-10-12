#pragma once
#include "Polygn.h"
#include <list>
#include "Camera.h"
#include <string.h>
#include "PointLight.h"

using namespace std;

namespace TakeOne
{
	const float PHONG_EXP = 5.0f;
	const float TEX_PHONG_EXP = 2.0f;
	const BYTE AMBIENT = 30;
	const float TEX_AMBIENT = 0.2f;
	const int MAX_POLY_SIZE = 800;
	const float BUMP_POWER = 0.6f;
	const float ROT_AMOUNT = 0.5f;

	struct DrawInfo
	{
		Matrix * worldMatrix;
		Matrix * rotationOnly;
		Polygn* polys;
		Vertex* vert;
		Vertex* transVerts;
		int numPolys;
		int numVerts;
		bool textured;
		
	};
	struct EndPoint
	{
		int x;
		int y;
		float light;
		float spec;
		POINT uv;
		Vector normal;
		Vector eye;
		float _r;
		float _b;
		float _g;
	};
	class Rasterizer
	{
	public:
		Rasterizer(Camera *);
		~Rasterizer(void);

	public://Draw Calls
		void SubmitToDraw(DrawInfo& info);
		void Draw(HDC&, Camera&);
		void DrawPhong(HDC&, Camera&);
		void DrawFence(HDC&, Camera&);
		void DrawFlat(HDC&, Camera&);
		void DrawWireFrame(HDC&, Camera&);
		void ClearInfo();
		void MutateLights();
		void MakePointLights();
		Gdiplus::Bitmap * _bitmap;
		
	private://Flat shading
		void ProcessVectorsFlat(DrawInfo& info, Camera&);
		void DrawPolyFlat(Polygn*);
		void DrawPolyWire(Polygn*);

	private:// Lit rendering
		void ProcessVectors(DrawInfo& info, Camera&);
		void ProcessPolys(DrawInfo& info, Camera&);
		void DepthSortPolys();
		void ScanPoly(Polygn*);
		int TraceEdge(Vertex* one, Vertex* two, EndPoint *, Polygn*, int, bool endLine);
		void ScanLine(EndPoint* one, EndPoint* two, Polygn* p);
		void InterpolDraw(Polygn*, int x, int y);
		void InterpolDrawNoCheck(Polygn*, int x, int y);
		void RGBDraw(Polygn*, int x, int y);
		void RGBDrawNoCheck(Polygn*, int x, int y);

	private://Textured/Lit rendering
		void ScanPolyTex(Polygn* p);
		void ScanLineTex(EndPoint* one, EndPoint* two, Polygn * p);
		int TraceEdgeTex(Vertex* one, Vertex* two, EndPoint * arr, Polygn* p, int offset);
		void InterpolDrawTex(Polygn* p, int x, int y);
		void InterpolDrawNoCheckTex(Polygn* p, int x, int y);

	private: //Phong Shading
		void ScanPolyPhong(Polygn* p);
		void ScanLinePhong(EndPoint* one, EndPoint* two, Polygn * p);
		int TraceEdgePhong(Vertex* one, Vertex* two, EndPoint * arr, Polygn * p, int offset);
		void ProcessVectorsPhong(DrawInfo& info, Camera&);

	private://Textured Phong
		void ScanPolyPhongTex(Polygn *p);
		void ScanLinePhongTex(EndPoint* one, EndPoint* two, Polygn * p);
		int TraceEdgePhongTex(Vertex* one, Vertex* two, EndPoint * arr, Polygn * p, int offset);

	private: //bumpmapped phong
		void ScanPolyPhongBump(Polygn *p);
		void ScanLinePhongBump(EndPoint* one, EndPoint* two, Polygn * p);
		int TraceEdgePhongBump(Vertex* one, Vertex* two, EndPoint * arr, Polygn * p, int offset);

	private: //Fence Shading
		void ScanPolyFence(Polygn* p);
		int TraceEdgeFence(Vertex* one, Vertex* two, EndPoint * arr, Polygn * p, int offset);
		void ScanLineFence(EndPoint* one, EndPoint* two, Polygn * p);

	private://Textured Fence
		void ScanPolyFenceTex(Polygn *p);
		int TraceEdgeFenceTex(Vertex* one, Vertex* two, EndPoint * arr, Polygn * p, int offset);


	private://minor funcs
		void SetLeftTopRight(Polygn* p);
		void Swap(int& a, int& b);
		float Distance(int, int, float, float);
		bool IsInsideX(int x);
		bool IsInsideY(int y);
		bool OnScreen(Polygn*);
		void SpinLights();
		
	
	public:
		int _onScrPolys, _onScrVerts;
		std::string _CurrentRender;


	private:
		list<DrawInfo*> _info;
		list<Polygn*> _validPolys;
		Vector _lightDir;
		PointLight _lights[3];
		//Scanning stuff
		EndPoint _st[1000];
		EndPoint _ed[1000];
		Vertex* _vtop, *_vleft, *_vright;
		HDC* _tempHdc;
		Gdiplus::Graphics * _tempG;
		Camera * _cam;
		Vector2 _currentVP;

		//interpol stuff
		BYTE _br, _bg, _bb;
		BYTE _light;
		BYTE _spec;
		float _subDx;
		float _flight;
		float _fspec;
		float _dx;
		float _dy;
		Gdiplus::BitmapData * _bmd;
		BYTE * _row;
		BYTE * _temp;
		BYTE * _lTemp;
		BYTE * _bump;
		float _r;
		float _b;
		float _g;

		//poly stuff
		int startCount;
		int endCount;
		int deltaY;
		int deltaX;
		int error;
		int xStep;

		//Tex stuff
		int _u;
		int _v;

		//additional phongy stuff
		int x0, x1, y0, y1;
		Vector _eye;
		Vector _nrm;
		Vector _reflection;
		float dp;
		Vector _bumpVec;

		//Flat shading stuff
		Gdiplus::PointF _points[3];

	};
}
