#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include "GraphObject.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

Actor* StudentWorld::getActor(int x, int y)
{
	for (auto p : m_actors)
	{
		if (p->getX() == x && p->getY() == y)
			return p;
	}

	return nullptr;
}

Actor* StudentWorld::getOtherActor(int x, int y)
{
	for (std::list<Actor*>::reverse_iterator p = m_actors.rbegin(); p != m_actors.rend(); p++)
	{
		if ((*p)->getX() == x && (*p)->getY() == y)
			return *p;
	}

	return nullptr;
}

void StudentWorld::setDisplayText()
{
	ostringstream oss;
	oss.fill('0');
	oss << "Score: " << setw(7) << getScore() << "  ";
	oss << "Level: " << setw(2) << getLevel() << "  ";
	oss.fill(' ');
	oss << "Lives: " << setw(2) << getLives() << "  ";
	oss << "Health: " << setw(3) << m_player->getHealthAsPercentage() << "  ";
	oss << "Ammo: " << setw(3) << m_player->getAmmo() << "  ";
	oss << "Bonus: " << setw(4) << m_bonus;
	string s = oss.str();
	setGameStatText(s);
}

int StudentWorld::init()
{
	if (getLevel() > 99)
		return GWSTATUS_PLAYER_WON;

	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << getLevel() << ".dat";
	string curLevel = oss.str();
	Level lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(curLevel);

	if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;

	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;

	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			Level::MazeEntry item = lev.getContentsOf(x, y);
			switch (item)
			{
			case Level::player:
				m_player = new Player(x, y, this);
				break;
			case Level::wall:
				m_actors.push_back(new Wall(x, y, this));
				break;
			case Level::boulder:
				m_actors.push_back(new Boulder(x, y, this));
				break;
			case Level::hole:
				m_actors.push_back(new Hole(x, y, this));
				break;
			case Level::jewel:
				m_actors.push_back(new Jewel(x, y, this));
				break;
			case Level::exit:
				m_actors.push_back(new Exit(x, y, this));
				break;
			case Level::extra_life:
				m_actors.push_back(new ExtraLifeGoodie(x, y, this));
				break;
			case Level::restore_health:
				m_actors.push_back(new RestoreHealthGoodie(x, y, this));
				break;
			case Level::ammo:
				m_actors.push_back(new AmmoGoodie(x, y, this));
				break;
			case Level::horiz_snarlbot:
				m_actors.push_back(new SnarlBot(x, y, GraphObject::Direction::right, this));
				break;
			case Level::vert_snarlbot:
				m_actors.push_back(new SnarlBot(x, y, GraphObject::Direction::down, this));
				break;
			case Level::kleptobot_factory:
				m_actors.push_back(new KleptoBotFactory(x, y, this, 0));
				break;
			case Level::angry_kleptobot_factory:
				m_actors.push_back(new KleptoBotFactory(x, y, this, 1));
				break;
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_player->doSomething();

	for (std::list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
	{
		(*p)->doSomething();
	}

	if (!m_player->isAlive())
	{
		decLives();
		m_bonus = START_BONUS;
		return GWSTATUS_PLAYER_DIED;
	}

	for (std::list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end();)
	{
		if (!(*p)->isAlive())
		{
			delete *p;
			std::list<Actor*>::iterator q = m_actors.erase(p);
			p = q;
			continue;
		}
		p++;
	}

	if (m_bonus > 0)
		m_bonus--;

	if (isFinished())
	{
		endOfLevel = false;
		increaseScore(m_bonus);
		
		return GWSTATUS_FINISHED_LEVEL;	
	}

	setDisplayText();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (std::list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end();)
	{
		delete *p;
		std::list<Actor*>::iterator q = m_actors.erase(p);
		p = q;
	}

	delete m_player;
	m_player = nullptr;
}

int StudentWorld::getJewelCount()
{
	int count = 0;

	for (auto p : m_actors)
	{
		Jewel* jp = dynamic_cast<Jewel*>(p);

		if (jp != nullptr) count++;
	}

	return count;
}

int StudentWorld::computeSnarlBotTicks()
{
	int ticks = (28 - getLevel()) / 4;
	if (ticks < 3)
		ticks = 3;

	return ticks;
}