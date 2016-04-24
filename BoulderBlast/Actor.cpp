#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

void Bullet::doSomething()
{
	if (firstStep)
	{
		firstStep = false;
		return;
	}

	switch (getDirection())
	{
	case left:
	{
		if (getX() - 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
		{
			getWorld()->getPlayer()->getHit();
			setDead();
			break;
		}

		Actor* ap = getWorld()->getActor(getX() - 1, getY());
		if (ap == nullptr)
		{
			moveTo(getX() - 1, getY());
			break;
		}
		if (ap->allowsAgentColocation())
		{
			Actor* ap2 = getWorld()->getOtherActor(getX() - 1, getY());
			if (ap2 != nullptr)
			{
				ap2->getHit();
				setDead();
				break;
			}
		}
		if (ap->isDamageable())
		{
			ap->getHit();
			setDead();
			break;
		}
		if (ap->isSolid())
		{	
			setDead();
			break;
		}

		moveTo(getX() - 1, getY());
		break;
	}
	case right:
	{
		if (getX() + 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
		{
			getWorld()->getPlayer()->getHit();
			setDead();
			break;
		}

		Actor* ap = getWorld()->getActor(getX() + 1, getY());
		if (ap == nullptr)
		{
			moveTo(getX() + 1, getY());
			break;
		}
		if (ap->allowsAgentColocation())
		{
			Actor* ap2 = getWorld()->getOtherActor(getX() + 1, getY());
			if (ap2 != nullptr)
			{
				ap2->getHit();
				setDead();
				break;
			}
		}
		if (ap->isDamageable())
		{
			ap->getHit();
			setDead();
			break;
		}
		if (ap->isSolid())
		{
			setDead();
			break;
		}

		moveTo(getX() + 1, getY());
		break;
	}
	case up:
	{
		if (getX() == getWorld()->getPlayer()->getX() && getY() + 1 == getWorld()->getPlayer()->getY())
		{
			getWorld()->getPlayer()->getHit();
			setDead();
			break;
		}

		Actor* ap = getWorld()->getActor(getX(), getY() + 1);
		if (ap == nullptr)
		{
			moveTo(getX(), getY() + 1);
			break;
		}
		if (ap->allowsAgentColocation())
		{
			Actor* ap2 = getWorld()->getOtherActor(getX(), getY() + 1);
			if (ap2 != nullptr)
			{
				ap2->getHit();
				setDead();
				break;
			}
		}
		if (ap->isDamageable())
		{
			ap->getHit();
			setDead();
			break;
		}
		if (ap->isSolid())
		{
			setDead();
			break;
		}

		moveTo(getX(), getY() + 1);
		break;
	}
	case down:
	{
		if (getX() == getWorld()->getPlayer()->getX() && getY() - 1 == getWorld()->getPlayer()->getY())
		{
			getWorld()->getPlayer()->getHit();
			setDead();
			break;
		}

		Actor* ap = getWorld()->getActor(getX(), getY() - 1);

		if (ap == nullptr)
		{
			moveTo(getX(), getY() - 1);
			break;
		}
		if (ap->allowsAgentColocation())
		{
			Actor* ap2 = getWorld()->getOtherActor(getX(), getY() - 1);
			if (ap2 != nullptr)
			{
				ap2->getHit();
				setDead();
				break;
			}
		}
		if (ap->isDamageable())
		{
			ap->getHit();
			setDead();
			break;
		}
		if (ap->isSolid())
		{
			setDead();
			break;
		}

		moveTo(getX(), getY() - 1);
		break;
	}
	
	}
}

void Player::doSomething()
{
	if (!isAlive()) return;
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
		{
			setDirection(left);
			Actor* ap = getWorld()->getActor(getX() - 1, getY());
			if (ap != nullptr)
			{
				Boulder* bp = dynamic_cast<Boulder*>(ap);

				if (bp != nullptr)
				{
					if(bp->push())
						moveTo(getX() - 1, getY());
					break;
				}

				if (ap->isSolidToAgents() && ap->isSolidToPlayer())
					break;
			}
			moveTo(getX() - 1, getY());
			break;
		}
		case KEY_PRESS_RIGHT:
		{
			setDirection(right);
			Actor* ap = getWorld()->getActor(getX() + 1, getY());
			if (ap != nullptr)
			{
				Boulder* bp = dynamic_cast<Boulder*>(ap);

				if (bp != nullptr)
				{
					if (bp->push())
						moveTo(getX() + 1, getY());
					break;
				}

				if (ap->isSolidToAgents() && ap->isSolidToPlayer())
					break;
			}
			moveTo(getX() + 1, getY());
			break;
		}
		case KEY_PRESS_UP:
		{
			setDirection(up);
			Actor* ap = getWorld()->getActor(getX(), getY() + 1);
			if (ap != nullptr)
			{
				Boulder* bp = dynamic_cast<Boulder*>(ap);

				if (bp != nullptr)
				{
					if (bp->push())
						moveTo(getX(), getY() + 1);
					break;
				}

				if (ap->isSolidToAgents() && ap->isSolidToPlayer())
					break;
			}
			moveTo(getX(), getY() + 1);
			break;
		}
		case KEY_PRESS_DOWN:
		{
			setDirection(down);
			Actor* ap = getWorld()->getActor(getX(), getY() - 1);
			if (ap != nullptr)
			{
				Boulder* bp = dynamic_cast<Boulder*>(ap);

				if (bp != nullptr)
				{
					if (bp->push())
						moveTo(getX(), getY() - 1);
					break;
				}

				if (ap->isSolidToAgents() && ap->isSolidToPlayer())
					break;
			}
			moveTo(getX(), getY() - 1);
			break;
		}
		case KEY_PRESS_SPACE:
		{
			if (m_ammo < 1)
				break;
			m_ammo--;
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			Direction dir = getDirection();
			switch (dir)
			{
			case left:
			{
				Actor* ap = getWorld()->getActor(getX() - 1, getY());
				Actor* ap2 = getWorld()->getOtherActor(getX() - 1, getY());
				if (ap == nullptr)
				{
					getWorld()->getActorList()->push_back(new Bullet(getX() - 1, getY(), left, getWorld()));
					break;
				}
				if (ap->allowsAgentColocation())
				{
					if (ap2->isDamageable())
					{
						ap2->getHit();
						break;
					}
				}
				if (!ap->isSolid())
				{
					getWorld()->getActorList()->push_back(new Bullet(getX() - 1, getY(), left, getWorld()));
					break;
				}
				if (ap->isDamageable())
				{
					ap->getHit();
					break;
				}
				break;
			}
			case right:
			{
				Actor* ap = getWorld()->getActor(getX() + 1, getY());
				Actor* ap2 = getWorld()->getOtherActor(getX() + 1, getY());
				if (ap == nullptr)
				{
					getWorld()->getActorList()->push_back(new Bullet(getX() + 1, getY(), right, getWorld()));
					break;
				}
				if (ap->allowsAgentColocation())
				{
					if (ap2->isDamageable())
					{
						ap2->getHit();
						break;
					}
				}
				if (!ap->isSolid())
				{
					getWorld()->getActorList()->push_back(new Bullet(getX() + 1, getY(), right, getWorld()));
					break;
				}
				if (ap->isDamageable())
				{
					ap->getHit();
					break;
				}
				break;
			}
			case up:
			{
				Actor* ap = getWorld()->getActor(getX(), getY() + 1);
				Actor* ap2 = getWorld()->getOtherActor(getX(), getY() + 1);
				if (ap == nullptr)
				{
					getWorld()->getActorList()->push_back(new Bullet(getX(), getY() + 1, up, getWorld()));
					break;
				}
				if (ap->allowsAgentColocation())
				{
					if (ap2->isDamageable())
					{
						ap2->getHit();
						break;
					}
				}
				if (!ap->isSolid())
				{
					getWorld()->getActorList()->push_back(new Bullet(getX(), getY() + 1, up, getWorld()));
					break;
				}
				if (ap->isDamageable())
				{
					ap->getHit();
					break;
				}
				break;
			}
			case down:
			{
				Actor* ap = getWorld()->getActor(getX(), getY() - 1);
				Actor* ap2 = getWorld()->getOtherActor(getX(), getY() - 1);
				if (ap == nullptr)
				{
					getWorld()->getActorList()->push_back(new Bullet(getX(), getY() - 1, down, getWorld()));
					break;
				}
				if (ap->allowsAgentColocation())
				{
					if (ap2->isDamageable())
					{
						ap2->getHit();
						break;
					}
				}
				if (!ap->isSolid())
				{
					getWorld()->getActorList()->push_back(new Bullet(getX(), getY() - 1, down, getWorld()));
					break;
				}
				if (ap->isDamageable())
				{
					ap->getHit();
					break;
				}
				break;
			}
			}
			break;
		}
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
		}
	}
}

void Player::getHit()
{
	decreaseHealth(2);
	if (getHealth() > 0)
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
	else
	{
		getWorld()->playSound(SOUND_PLAYER_DIE);
		setDead();
	}
}

std::string Player::getHealthAsPercentage()
{
	int percentage = getHealth() * 5;
	std::ostringstream oss;
	oss.fill(' ');
	oss << std::setw(2) << percentage << '%';
	std::string result = oss.str();
	return result;
}

bool Boulder::push()
{
	switch (getWorld()->getPlayer()->getDirection())
	{
	case left:
	{
		Actor* ap = getWorld()->getActor(getX() - 1, getY());
		if (ap == nullptr)
		{
			moveTo(getX() - 1, getY());
			return true;
		}
		if (ap->isAbleToSwallow())
		{
			moveTo(getX() - 1, getY());
			ap->setDead();
			this->setDead();
			return true;
		}
		if (ap->isSolidToBoulders()) break;
	}
	case right:
	{
		Actor* ap = getWorld()->getActor(getX() + 1, getY());
		if (ap == nullptr)
		{
			moveTo(getX() + 1, getY());
			return true;
		}
		if (ap->isAbleToSwallow())
		{
			moveTo(getX() + 1, getY());
			ap->setDead();
			this->setDead();
			return true;
		}
		if (ap->isSolidToBoulders()) break;
	}
	case up:
	{
		Actor* ap = getWorld()->getActor(getX(), getY() + 1);
		if (ap == nullptr)
		{
			moveTo(getX(), getY() + 1);
			return true;
		}
		if (ap->isAbleToSwallow())
		{
			moveTo(getX(), getY() + 1);
			ap->setDead();
			this->setDead();
			return true;
		}
		if (ap->isSolidToBoulders()) break;
	}
	case down:
	{
		Actor* ap = getWorld()->getActor(getX(), getY() - 1);
		if (ap == nullptr)
		{
			moveTo(getX(), getY() - 1);
			return true;
		}
		if (ap->isAbleToSwallow())
		{
			moveTo(getX(), getY() - 1);
			ap->setDead();
			this->setDead();
			return true;
		}
		if (ap->isSolidToBoulders()) break;
	}
	}
	
	return false;
}

void Boulder::getHit()
{
	decreaseHealth(2);
}

void Jewel::doSomething()
{
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
	}
}

void Exit::doSomething()
{
	if (!isRevealed && getWorld()->getJewelCount() == 0)
	{
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		setVisible(true);
		isRevealed = true;
	}

	if (isRevealed && getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000);
		getWorld()->levelComplete();
	}
}

void ExtraLifeGoodie::doSomething()
{
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY() && isVisible())
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(1000);
		getWorld()->incLives();
	}
}

void RestoreHealthGoodie::doSomething()
{
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY() && isVisible())
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(500);
		getWorld()->getPlayer()->setHealth(20);
	}
}

void AmmoGoodie::doSomething()
{
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY() && isVisible())
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(100);
		getWorld()->getPlayer()->increaseAmmo(20);
	}
}

void SnarlBot::doSomething()
{
	maxTick = getWorld()->computeSnarlBotTicks();

	if (currentTick != maxTick)
	{
		currentTick++;
		return;
	}

	switch (getDirection())
	{
	case left:
	{
		if (getY() == getWorld()->getPlayer()->getY() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() - 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				break;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX() - 1, getY(), left, getWorld()));
			break;
		}

		Actor* ap = getWorld()->getActor(getX() - 1, getY());
		if (ap == nullptr)
		{
			moveTo(getX() - 1, getY());
			break;
		}
		if (ap->isSolidToAgents())
		{
			setDirection(right);
			break;
		}
		moveTo(getX() - 1, getY());
		break;
	}
	case right:
	{
		if (getY() == getWorld()->getPlayer()->getY() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() + 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				break;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX() + 1, getY(), right, getWorld()));
			break;
		}

		Actor* ap = getWorld()->getActor(getX() + 1, getY());
		if (ap == nullptr)
		{
			moveTo(getX() + 1, getY());
			break;
		}
		if (ap->isSolidToAgents())
		{
			setDirection(left);
			break;
		}
		moveTo(getX() + 1, getY());
		break;
	}
	case up:
	{
		if (getX() == getWorld()->getPlayer()->getX() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() == getWorld()->getPlayer()->getX() && getY() + 1 == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				break;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX(), getY() + 1, up, getWorld()));
			break;
		}

		Actor* ap = getWorld()->getActor(getX(), getY() + 1);
		if (ap == nullptr)
		{
			moveTo(getX(), getY() + 1);
			break;
		}
		if (ap->isSolidToAgents())
		{
			setDirection(down);
			break;
		}
		moveTo(getX(), getY() + 1);
		break;
	}
	case down:
	{
		if (getX() == getWorld()->getPlayer()->getX() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() == getWorld()->getPlayer()->getX() && getY() - 1 == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				break;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX(), getY() - 1, down, getWorld()));
			break;
		}

		Actor* ap = getWorld()->getActor(getX(), getY() - 1);
		if (ap == nullptr)
		{
			moveTo(getX(), getY() - 1);
			break;
		}
		if (ap->isSolidToAgents())
		{
			setDirection(up);
			break;
		}
		moveTo(getX(), getY() - 1);
		break;
	}
	}

	currentTick = 0;
}

bool SnarlBot::isClearShot()
{
	int curr;
	switch (getDirection())
	{
	case left:
	case right:
		curr = getX();
		break;
	case up:
	case down:
		curr = getY();
		break;
	}

	int player;
	switch (getDirection())
	{
	case left:
	case right:
		player = getWorld()->getPlayer()->getX();
		break;
	case up:
	case down:
		player = getWorld()->getPlayer()->getY();
		break;
	}

	while (curr != player)
	{
		switch (getDirection())
		{
		case left:
		{
			curr--;
			Actor* ap = getWorld()->getActor(curr, getY());
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		case right:
		{
			curr++;
			Actor* ap = getWorld()->getActor(curr, getY());
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		case up:
		{
			curr++;
			Actor* ap = getWorld()->getActor(getX(), curr);
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		case down:
		{
			curr--;
			Actor* ap = getWorld()->getActor(getX(), curr);
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		}
	}

	return true;
}

void SnarlBot::getHit()
{
	decreaseHealth(2);
	if (getHealth() > 0)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else
	{
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(100);
	}
}

void KleptoBotFactory::doSomething()
{
	int xMin = getX() - 3;
	int xMax = getX() + 3;
	int yMin = getY() - 3;
	int yMax = getY() + 3;

	int botCount = 0;

	for (int x = xMin; x <= xMax; x++)
	{
		for (int y = yMin; y <= yMax; y++)
		{
			KleptoBot* kp = dynamic_cast<KleptoBot*>(getWorld()->getActor(x, y));
			if (kp != nullptr) botCount++;
		}
	}

	if (botCount < 3 && dynamic_cast<KleptoBot*>(getWorld()->getOtherActor(getX(), getY())) == nullptr)
	{
		int chance = rand() % 50;
		if (chance == 0)
		{
			switch (m_type)
			{
			case 0:
				getWorld()->getActorList()->push_back(new RegularKleptoBot(getX(), getY(), getWorld()));
				getWorld()->playSound(SOUND_ROBOT_BORN);
				break;
			case 1:
				getWorld()->getActorList()->push_back(new AngryKleptoBot(getX(), getY(), getWorld()));
			    getWorld()->playSound(SOUND_ROBOT_BORN);
				break;
			}
		}
	}
}

void RegularKleptoBot::doSomething()
{
	maxTick = getWorld()->computeSnarlBotTicks();

	if (currentTick != maxTick)
	{
		currentTick++;
		return;
	}

	Actor* ap = getWorld()->getActor(getX(), getY());
	if (ap->isPickupable() && !isHoldingGoodie)
	{
		if (rand() % 10 == 0)
		{
			myGoodie = ap;
			myGoodie->setVisible(false);
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			isHoldingGoodie = true;
			return;
		}
	}

	bool timeToPickNew = false;
	if (currentDistance == distanceBeforeTurning) timeToPickNew = true;
	switch (getDirection())
	{
	case left:
	{
		Actor* ap = getWorld()->getActor(getX() - 1, getY());
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() - 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	case right:
	{
		Actor* ap = getWorld()->getActor(getX() + 1, getY());
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() + 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	case up:
	{
		Actor* ap = getWorld()->getActor(getX(), getY() + 1);
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() == getWorld()->getPlayer()->getX() && getY() + 1 == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	case down:
	{
		Actor* ap = getWorld()->getActor(getX(), getY() - 1);
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() == getWorld()->getPlayer()->getX() && getY() - 1 == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	}

	if (!timeToPickNew)
	{
		switch (getDirection())
		{
		case left:
			moveTo(getX() - 1, getY());
			currentDistance++;
			break;
		case right:
			moveTo(getX() + 1, getY());
			currentDistance++;
			break;
		case up:
			moveTo(getX(), getY() + 1);
			currentDistance++;
			break;
		case down:
			moveTo(getX(), getY() - 1);
			currentDistance++;
			break;
		}
	}
	else
	{
		currentDistance = 0;
		distanceBeforeTurning = (rand() % 6) + 1;
		Direction firstDirection = static_cast<Direction>((rand() % 4) + 1);
		Direction newDirection = firstDirection;
		bool directionsChecked[4] = { false, false, false, false };
		for (int i = 0; i < 4; i++)
		{
			directionsChecked[newDirection - 1] = true;
			switch (newDirection)
			{
			case left:
			{
				Actor* ap = getWorld()->getActor(getX() - 1, getY());
				if (ap == nullptr)
				{
					if (getX() - 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX() - 1, getY());
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			case right:
			{
				Actor* ap = getWorld()->getActor(getX() + 1, getY());
				if (ap == nullptr)
				{
					if (getX() + 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX() + 1, getY());
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			case up:
			{
				Actor* ap = getWorld()->getActor(getX(), getY() + 1);
				if (ap == nullptr)
				{
					if (getX() == getWorld()->getPlayer()->getX() && getY() + 1 == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX(), getY() + 1);
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			case down:
			{
				Actor* ap = getWorld()->getActor(getX(), getY() - 1);
				if (ap == nullptr)
				{
					if (getX() == getWorld()->getPlayer()->getX() && getY() - 1 == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX(), getY() - 1);
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			}
		}
		failed:
		setDirection(firstDirection);
	done:
		;
	}


	currentTick = 0;
}

void RegularKleptoBot::getHit()
{
	decreaseHealth(2);
	if (getHealth() > 0)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else
	{
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(10);
		if (isHoldingGoodie)
		{
			myGoodie->setVisible(true);
			myGoodie->moveTo(getX(), getY());
		}
	}
}

void AngryKleptoBot::doSomething()
{
	maxTick = getWorld()->computeSnarlBotTicks();

	if (currentTick != maxTick)
	{
		currentTick++;
		return;
	}

	switch (getDirection())
	{
	case left:
		if (getY() == getWorld()->getPlayer()->getY() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() - 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				currentTick = 0;
				return;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX() - 1, getY(), left, getWorld()));
			currentTick = 0;
			return;
		}
		break;
	case right:
		if (getY() == getWorld()->getPlayer()->getY() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() + 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				currentTick = 0;
				return;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX() + 1, getY(), right, getWorld()));
			currentTick = 0;
			return;
		}
		break;
	case up:
		if (getX() == getWorld()->getPlayer()->getX() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() == getWorld()->getPlayer()->getX() && getY() + 1 == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				currentTick = 0;
				return;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX(), getY() + 1, up, getWorld()));
			currentTick = 0;
			return;
		}
		break;
	case down:
		if (getX() == getWorld()->getPlayer()->getX() && isClearShot())
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			if (getX() == getWorld()->getPlayer()->getX() && getY() - 1 == getWorld()->getPlayer()->getY())
			{
				getWorld()->getPlayer()->getHit();
				currentTick = 0;
				return;
			}
			getWorld()->getActorList()->push_back(new Bullet(getX(), getY() - 1, down, getWorld()));
			currentTick = 0;
			return;
		}
		break;
	}

	Actor* ap = getWorld()->getActor(getX(), getY());
	if (ap->isPickupable() && !isHoldingGoodie)
	{
		if (rand() % 10 == 0)
		{
			myGoodie = ap;
			myGoodie->setVisible(false);
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			isHoldingGoodie = true;
			return;
		}
	}

	bool timeToPickNew = false;
	if (currentDistance == distanceBeforeTurning) timeToPickNew = true;
	switch (getDirection())
	{
	case left:
	{
		Actor* ap = getWorld()->getActor(getX() - 1, getY());
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() - 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	case right:
	{
		Actor* ap = getWorld()->getActor(getX() + 1, getY());
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() + 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	case up:
	{
		Actor* ap = getWorld()->getActor(getX(), getY() + 1);
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() == getWorld()->getPlayer()->getX() && getY() + 1 == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	case down:
	{
		Actor* ap = getWorld()->getActor(getX(), getY() - 1);
		if (ap != nullptr && ap->isSolidToAgents()) timeToPickNew = true;
		if (getX() == getWorld()->getPlayer()->getX() && getY() - 1 == getWorld()->getPlayer()->getY()) timeToPickNew = true;
		break;
	}
	}

	if (!timeToPickNew)
	{
		switch (getDirection())
		{
		case left:
			moveTo(getX() - 1, getY());
			currentDistance++;
			break;
		case right:
			moveTo(getX() + 1, getY());
			currentDistance++;
			break;
		case up:
			moveTo(getX(), getY() + 1);
			currentDistance++;
			break;
		case down:
			moveTo(getX(), getY() - 1);
			currentDistance++;
			break;
		}
	}
	else
	{
		currentDistance = 0;
		distanceBeforeTurning = (rand() % 6) + 1;
		Direction firstDirection = static_cast<Direction>((rand() % 4) + 1);
		Direction newDirection = firstDirection;
		bool directionsChecked[4] = { false, false, false, false };
		for (int i = 0; i < 4; i++)
		{
			directionsChecked[newDirection - 1] = true;
			switch (newDirection)
			{
			case left:
			{
				Actor* ap = getWorld()->getActor(getX() - 1, getY());
				if (ap == nullptr)
				{
					if (getX() - 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX() - 1, getY());
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			case right:
			{
				Actor* ap = getWorld()->getActor(getX() + 1, getY());
				if (ap == nullptr)
				{
					if (getX() + 1 == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX() + 1, getY());
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			case up:
			{
				Actor* ap = getWorld()->getActor(getX(), getY() + 1);
				if (ap == nullptr)
				{
					if (getX() == getWorld()->getPlayer()->getX() && getY() + 1 == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX(), getY() + 1);
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			case down:
			{
				Actor* ap = getWorld()->getActor(getX(), getY() - 1);
				if (ap == nullptr)
				{
					if (getX() == getWorld()->getPlayer()->getX() && getY() - 1 == getWorld()->getPlayer()->getY())
						break;
					setDirection(newDirection);
					moveTo(getX(), getY() - 1);
					goto done;
				}
				if (ap->isSolidToAgents())
				{
					bool allDone = true;
					for (int i = 0; i < 4; i++)
						if (!directionsChecked[i]) allDone = false;
					if (allDone)
						goto failed;

					while (directionsChecked[newDirection - 1])
					{
						newDirection = static_cast<Direction>((rand() % 4) + 1);
					}

					break;
				}
			}
			}
		}
	failed:
		setDirection(firstDirection);
	done:
		;
	}


	currentTick = 0;
}

void AngryKleptoBot::getHit()
{
	decreaseHealth(2);
	if (getHealth() > 0)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else
	{
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(20);
		if (isHoldingGoodie)
		{
			myGoodie->setVisible(true);
			myGoodie->moveTo(getX(), getY());
		}
	}
}

bool AngryKleptoBot::isClearShot()
{
	int curr;
	switch (getDirection())
	{
	case left:
	case right:
		curr = getX();
		break;
	case up:
	case down:
		curr = getY();
		break;
	}

	int player;
	switch (getDirection())
	{
	case left:
	case right:
		player = getWorld()->getPlayer()->getX();
		break;
	case up:
	case down:
		player = getWorld()->getPlayer()->getY();
		break;
	}

	while (curr != player)
	{
		switch (getDirection())
		{
		case left:
		{
			curr--;
			Actor* ap = getWorld()->getActor(curr, getY());
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		case right:
		{
			curr++;
			Actor* ap = getWorld()->getActor(curr, getY());
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		case up:
		{
			curr++;
			Actor* ap = getWorld()->getActor(getX(), curr);
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		case down:
		{
			curr--;
			Actor* ap = getWorld()->getActor(getX(), curr);
			if (ap == nullptr) break;
			if (ap->isSolid()) return false;
			break;
		}
		}
	}

	return true;
}