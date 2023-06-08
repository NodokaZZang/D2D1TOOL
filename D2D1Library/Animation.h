#pragma once
class Animation : public ResourceObj
{
private:
	std::vector<Sprite*> m_clips;
	int	m_nowFrame = 0;
	Sprite* m_nowSprite;
public:
	Animation() : ResourceObj(ANIMATION) { }
	~Animation();
	void AddClip(Sprite* _sprite);
	Sprite* GetFrame();
	Sprite* GetFrameNow();
	int		GetFrameCount() { return m_clips.size(); }
	std::vector<Sprite*>& GetClips() { return m_clips; };
};

