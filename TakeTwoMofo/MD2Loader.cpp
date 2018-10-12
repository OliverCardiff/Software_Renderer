#include "StdAfx.h"
#include "MD2Loader.h"
#include "Vector.h"

// file reading
#include <iostream>
#include <fstream>

#include "Bone.h"

typedef unsigned char BYTE; // added in case not doing in a win32 app

using namespace TakeOne;

// magic number "IDP2" or 844121161
const int MD2_IDENT = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I');

// Bone version
const int MD2_VERSION = 8;

struct Md2Header_t
{
      int indent;             // The magic number used to identify the file.
      int version;            // The file version number (must be 8).
      int skinWidth;          // The width in pixels of our image.
      int skinHeight;         // The height in pixels of our image.
      int frameSize;          // The size in bytes the frames are.
      int numSkins;           // The number of skins associated with the Bone.
      int numVertices;		  // The number of vertices.
      int numTexCoords;		  // The number of texture coordinates.
      int numTriangles;		  // The number of faces (polygons).
      int numGlCommands;      // The number of gl commands.
      int numFrames;          // The number of animated frames.
      int offsetSkins;		  // The offset in the file for the skin data.
      int offsetTexCoords;	  // The offset in the file for the texture data.
      int offsetTriangles;	  // The offset in the file for the face data.
      int offsetFrames;		  // The offset in the file for the frames data.
      int offsetGlCommands;	  // The offset in the file for the gl commands data.
      int offsetEnd;          // The end of the file offset.
};

// triangle
struct Md2Triangle_t
{
	short vertexIndex[3];   // vertex indices of the triangle
	short uvIndex[3];       // tex. coord. indices 
};

// vertex
struct Md2Vertex_t
{
    BYTE v[3];                // compressed vertex (x, y, z) coordinates
    BYTE lightnormalindex;    // index to a normal vector for the lighting

};

// frame
struct Md2Frame_t
{
    float       scale[3];       // scale values
    float       translate[3];   // translation vector
    char        name[16];       // frame name
    Md2Vertex_t verts[1];       // first vertex of this frame
};

MD2Loader::MD2Loader(void)
{
}

MD2Loader::~MD2Loader(void)
{
}

// ----------------------------------------------
// LoadBone() - load Bone from file.
// ----------------------------------------------
bool MD2Loader::LoadModel( const char* filename, TakeOne::Bone& obj )
{
    std::ifstream   file;           // file stream

	// try to open filename
    file.open( filename, std::ios::in | std::ios::binary );

    if( file.fail() )
        return false;

    // read md2 header file
    Md2Header_t header;         
	file.read( reinterpret_cast<char*>(&header), sizeof( Md2Header_t ) );

    /////////////////////////////////////////////
    //      verify that this is a MD2 file

    // check for the ident and the version number
    if( (header.indent != MD2_IDENT) && (header.version != MD2_VERSION) )
    {
        // this is not a MD2 Bone
        file.close();
        return false;
    }

    /////////////////////////////////////////////

    // allocate memory dynamically (on heap)
	Md2Triangle_t* pTriangles = new Md2Triangle_t[ header.numTriangles ];
	BYTE* pFrameBuffer = new BYTE[ header.frameSize ];// only interested in one frame (first)
	Md2Frame_t* pFrame = reinterpret_cast<Md2Frame_t*>(pFrameBuffer);

    /////////////////////////////////////////////
    //          reading file data

	// read polygon data...
	file.seekg( header.offsetTriangles, std::ios::beg );
	file.read( reinterpret_cast<char*>(pTriangles), sizeof(Md2Triangle_t) * header.numTriangles );	// read just first frame

    // read frame data...
    file.seekg( header.offsetFrames, std::ios::beg );
    file.read( reinterpret_cast<char*>(pFrame), header.frameSize );	// read just first frame

	// close the file 
    file.close();
    /////////////////////////////////////////////
	
	obj.BuildArrays(header.numTriangles, header.numVertices);

	// polygon array initialization
	for ( int i = 0; i < header.numTriangles; ++i )
	{
		obj._polys[i]._indices[0] = &obj._transVerts[pTriangles[i].vertexIndex[0]];
		obj._polys[i]._indices[1] = &obj._transVerts[pTriangles[i].vertexIndex[1]];
		obj._polys[i]._indices[2] = &obj._transVerts[pTriangles[i].vertexIndex[2]];
		obj._polys[i].textured = false;
	}

	// vertex array initialization
    for( int i = 0; i < header.numVertices; ++i )
	{
		// NOTE: we have to swap Y and Z over because Z is up in Quake2 and we have Y as up-axis
		obj._verts[i]._vec._x = (pFrame->verts[i].v[0] * pFrame->scale[0]) + pFrame->translate[0];
		obj._verts[i]._vec._z = (pFrame->verts[i].v[1] * pFrame->scale[1]) + pFrame->translate[1];
		obj._verts[i]._vec._y = (pFrame->verts[i].v[2] * pFrame->scale[2]) + pFrame->translate[2];
		obj._verts[i]._vec._w = 1.0f ; // DON'T FORGET THIS otherwise looks 'exploded'

		obj._transVerts[i]._vec._x = (pFrame->verts[i].v[0] * pFrame->scale[0]) + pFrame->translate[0];
		obj._transVerts[i]._vec._z = (pFrame->verts[i].v[1] * pFrame->scale[1]) + pFrame->translate[1];
		obj._transVerts[i]._vec._y = (pFrame->verts[i].v[2] * pFrame->scale[2]) + pFrame->translate[2];
		obj._transVerts[i]._vec._w = 1.0f ;
	}

	// free buffer's heap memory
    delete [] pTriangles; // NOTE: this is 'array' delete must be sure to use this
	pTriangles = 0;

	delete [] pFrameBuffer;
	pFrameBuffer = 0;
	pFrame = 0;

	return true;
}