#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <list>
#include <string>

const int START_BONUS = 1000;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) {}

	~StudentWorld() { cleanUp(); }

	Player* getPlayer() { return m_player; }
	Actor* getActor(int x, int y);
	Actor* getOtherActor(int x, int y);
	std::list<Actor*>* getActorList() { return &m_actors; }
	void setDisplayText();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	int getJewelCount();
	bool isFinished() const { return endOfLevel; }
	void levelComplete() { endOfLevel = true; }
	int computeSnarlBotTicks();

private:
	std::list<Actor*> m_actors;
	Player* m_player;
	unsigned int m_bonus = 1000;
	bool endOfLevel = false;
};

#endif // STUDENTWORLD_H_