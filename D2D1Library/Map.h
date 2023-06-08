#pragma once
class Map : public ResourceObj
{ 
private:
	char m_fileName[256];
	char m_imgFileName[256];
	int m_xSize;
	int m_ySize;
	int m_mapData[256][256];
public:
	Map(char _fileName[], int _xSize, int _ySize, int** _mapData);
	Map(const MapDataBinaryFile& _data);
	~Map() {};

	void copyMapData(int** _map) 
	{
		for (int y = 0; y < m_ySize; y++)
			for (int x = 0; x < m_xSize; x++)
				_map[y][x] = m_mapData[y][x];
	}
	int  GetXSize() { return m_xSize; }
	int  GetYSize() { return m_ySize; }
	char* GetFileName() { return m_fileName; }
	char* GetImgFileName() { return m_imgFileName; }
private:
public:
};

