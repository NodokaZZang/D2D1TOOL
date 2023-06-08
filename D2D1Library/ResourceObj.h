#pragma once
enum RESOURCE_TYPE : int
{
	SPRITE = 0,
	ANIMATION = 1,
	EVENT = 2,
	MAP = 3,
	GAME_OBJECT = 4
};

class ResourceObj
{
public:
	ResourceObj(RESOURCE_TYPE _type) : m_type(_type) {}
	virtual ~ResourceObj() {}
protected:
	RESOURCE_TYPE m_type;
public:
	RESOURCE_TYPE GetResourceType() { return m_type; }
};

