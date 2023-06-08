#include "pch.h"
#include "Map.h"

Map::Map(char _fileName[], int _xSize, int _ySize, int** _mapData) : ResourceObj(MAP)
{
	strcpy_s(m_fileName, _fileName);
	m_xSize = _xSize;
	m_ySize = _ySize;

	for (int y = 0; y < m_ySize; y++)
		for (int x = 0; x < m_xSize; x++)
			m_mapData[y][x] = _mapData[y][x];
}

Map::Map(const MapDataBinaryFile& _data) : ResourceObj(MAP)
{
	strcpy_s(m_fileName, _data.fileName);
	strcpy_s(m_imgFileName, _data.imgFileName);
	m_xSize = _data.xSize;
	m_ySize = _data.ySize;

	for (int y = 0; y < m_ySize; y++)
		for (int x = 0; x < m_xSize; x++)
			m_mapData[y][x] = _data.mapData[y][x];
}
