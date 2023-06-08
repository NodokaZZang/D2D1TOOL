#pragma once
enum EVENT_TYPE : int
{
	EMPTY = 0,
	WALL = 1,
	Player = 2,
	Nefendes = 3,
	Ghost = 4,
	Kuma = 5,
	NefendesRect = 6,
	GhostRect = 7,
	KumaRect = 8
};
class Event : public ResourceObj
{
private:
	EVENT_TYPE m_event_type;
public:
	Event(EVENT_TYPE _event_type) : ResourceObj(EVENT), m_event_type(_event_type) {};

	EVENT_TYPE GetEventType() { return m_event_type; }

};

