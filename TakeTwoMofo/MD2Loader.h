#pragma once

// forward  declaration
namespace TakeOne { 
	class Bone; 

class MD2Loader
{
public:
	MD2Loader(void);
public:
	~MD2Loader(void);

public:
	static bool LoadModel(const char* pFilename, TakeOne::Bone&);
};
}