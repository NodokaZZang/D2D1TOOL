#pragma once

struct Pos
{
	int x;
	int y;
};

struct TargetSprite 
{
	int index;
	class Sprite* sprite;
	HTREEITEM	treeKey;
};

struct SpriteBinaryFileHeader 
{
	char fileName[256];
	int resourceType;
	int  spriteCount;
};

struct SpriteBinaryFileData
{
	D2D1_RECT_F		rect;
	D2D1_POINT_2F	pivotPos;
};


struct MapDataBinaryFile
{
	char imgFileName[256];
	char fileName[256];
	int xSize;
	int ySize;
	int mapData[256][256];
}; 

struct ObjDataBinaryFile
{
	char fileName[256];
	int  xPos;
	int  yPos;
	int  width;
	int  height;
	D2D1_POINT_2F pivotPos;
	int  objectCode;
};

enum ObjectType : int 
{
	PlayerObj = 100,
	NefendesObj = 200,
	GhostObj = 300,
	KumaObj = 400
};