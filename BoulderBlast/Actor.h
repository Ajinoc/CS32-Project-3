#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <string>

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction startDir, StudentWorld* world)
		: GraphObject(imageID, startX, startY, startDir), m_world(world)
	{
		setVisible(true);
	}

	virtual void doSomething() = 0;

	bool isAlive() const { return m_alive; }
	void setDead() { m_alive = false; }
	bool isSolid() const { return m_solid; }
	void setSolid(bool val) { m_solid = val; }
	StudentWorld* getWorld() const { return m_world; }

	virtual bool isSolidToAgents() { return true; }
	virtual bool isAbleToSwallow() { return false; }
	virtual bool isSolidToPlayer() { return true; }
	virtual bool isSolidToBoulders() { return true; }
	virtual bool isDamageable() { return false; }
	virtual bool allowsAgentColocation() { return false; }
	virtual bool isPickupable() { return false; }
	virtual void getHit() {}

private:
	StudentWorld* m_world;
	bool m_alive = true;
	bool m_solid = true;
};

	class Agent : public Actor
	{
	public:
		Agent(int imageID, int startX, int startY, Direction startDir, StudentWorld* world, int startingHealth)
			: Actor(imageID, startX, startY, startDir, world), m_health(startingHealth) {}

		virtual void doSomething() = 0;

		virtual bool isDamageable() { return true; }

		int getHealth() const { return m_health; }
		void decreaseHealth(int amt) { m_health -= amt; if (m_health <= 0) setDead(); }
		void setHealth(int amt) { m_health = amt; }

	private:
		int m_health;
	};

		class Player : public Agent
		{
		public:
			Player(int startX, int startY, StudentWorld* world)
				: Agent(IID_PLAYER, startX, startY, right, world, 20) {}

			virtual void doSomething();
			virtual void getHit();

			virtual bool isDamageable() { return true; }

			int getAmmo() const { return m_ammo; }
			void increaseAmmo(int amt) { m_ammo += amt; }
			std::string getHealthAsPercentage();

		private:
			int m_ammo = 20;
		};

		class Robot : public Agent
		{
		public:
			Robot(int imageID, int startX, int startY, Direction startDir, StudentWorld* world, int health)
				: Agent(imageID, startX, startY, startDir, world, health)  {}
		};

			class SnarlBot : public Robot
			{
			public:
				SnarlBot(int startX, int startY, Direction startDir, StudentWorld* world)
					: Robot(IID_SNARLBOT, startX, startY, startDir, world, 10) {}

				virtual void doSomething();

				virtual void getHit();

				bool isClearShot();

			private:
				int maxTick;
				int currentTick = 0;
			};

			class KleptoBot : public Robot
			{
			public:
				KleptoBot(int imageID, int startX, int startY, StudentWorld* world, int health)
					: Robot(imageID, startX, startY, right, world, health)
				{
					distanceBeforeTurning = (rand() % 6) + 1;
				}

				virtual bool isDamageable() { return true; }
				bool isHolding() const { return isHoldingGoodie; }
				void setHolding(bool val) { isHoldingGoodie = val; }

			protected:
				int maxTick;
				int currentTick = 0;
				int distanceBeforeTurning;
				int currentDistance = 0;
				bool isHoldingGoodie = false;
				Actor* myGoodie;
				
			};

				class RegularKleptoBot : public KleptoBot
				{
				public:
					RegularKleptoBot(int startX, int startY, StudentWorld* world)
						: KleptoBot(IID_KLEPTOBOT, startX, startY, world, 5) {}

					virtual void doSomething();
					virtual void getHit();
				};

				class AngryKleptoBot : public KleptoBot
				{
				public:
					AngryKleptoBot(int startX, int startY, StudentWorld* world)
						: KleptoBot(IID_ANGRY_KLEPTOBOT, startX, startY, world, 8) {}

					virtual void doSomething();
					virtual void getHit();

					bool isClearShot();
				};

	class Exit : public Actor
	{
	public:
		Exit(int startX, int startY, StudentWorld* world)
			: Actor(IID_EXIT, startX, startY, none, world) {
			setVisible(false); setSolid(false);
		}

		virtual void doSomething();

		virtual bool isSolidToBoulders() { return true; }
		virtual bool isSolidToAgents() { return false; }
		virtual bool isSolidToPlayer() { return false; }

	private:
		bool isRevealed = false;
	};

	class Wall : public Actor
	{
	public:
		Wall(int startX, int startY, StudentWorld* world)
			: Actor(IID_WALL, startX, startY, none, world) {}

		virtual void doSomething() {}
	};

	class Boulder : public Actor
	{
	public:
		Boulder(int startX, int startY, StudentWorld* world)
			: Actor(IID_BOULDER, startX, startY, none, world) {}

		virtual void doSomething() {}
		
		virtual bool isDamageable() { return true; }
		virtual void getHit();

		int getHealth() const { return m_health; }
		void decreaseHealth(int amt) { m_health -= amt; if (m_health <= 0) setDead(); }
		void setHealth(int amt) { m_health = amt; }
		bool push();

	private:
		int m_health = 10;
	};

	class Hole : public Actor
	{
	public:
		Hole(int startX, int startY, StudentWorld* world)
			: Actor(IID_HOLE, startX, startY, none, world) { setSolid(false); }

		virtual void doSomething() {}

		virtual bool isAbleToSwallow() { return true; }
	};

	class Bullet : public Actor
	{
	public:
		Bullet(int startX, int startY, Direction dir, StudentWorld* world)
			: Actor(IID_BULLET, startX, startY, dir, world) { setSolid(false); }

		virtual void doSomething();

		virtual bool isSolidToAgents() { return false; }
		virtual bool isSolidToPlayer() { return false; }
		virtual bool isSolidToBoulders() { return false; }

	private:
		bool firstStep = true;
	};

	class KleptoBotFactory : public Actor
	{
	public:
		KleptoBotFactory(int startX, int startY, StudentWorld* world, int type)
			: Actor(IID_ROBOT_FACTORY, startX, startY, none, world), m_type(type) {}

		virtual void doSomething();

		virtual bool allowsAgentColocation() { return true; }

	private:
		int m_type; // 0 for regular, 1 for angry
	};

	class PickupableItem : public Actor
	{
	public:
		PickupableItem(int imageID, int startX, int startY, StudentWorld* world)
			: Actor(imageID, startX, startY, none, world) {
			setSolid(false);
		}

		virtual void doSomething() = 0;

		virtual bool isSolidToAgents() { return false; }
		virtual bool isSolidToPlayer() { return false; }
		virtual bool allowsAgentColocation() { return true; }
	};

		class Jewel : public PickupableItem
		{
		public:
			Jewel(int startX, int startY, StudentWorld* world)
				: PickupableItem(IID_JEWEL, startX, startY, world) {}

			virtual void doSomething();
		};

		class Goodie : public PickupableItem
		{
		public:
			Goodie(int imageID, int startX, int startY, StudentWorld* world)
				: PickupableItem(imageID, startX, startY, world) { setSolid(false); }

			virtual void doSomething() = 0;

			virtual bool isPickupable() { return true; }
		};

			class ExtraLifeGoodie : public Goodie
			{
			public:
				ExtraLifeGoodie(int startX, int startY, StudentWorld* world)
					: Goodie(IID_EXTRA_LIFE, startX, startY, world) {}

				virtual void doSomething();
			};

		    class RestoreHealthGoodie : public Goodie
			{
			public:
				RestoreHealthGoodie(int startX, int startY, StudentWorld* world)
					: Goodie(IID_RESTORE_HEALTH, startX, startY, world) {}

				virtual void doSomething();
			};

			class AmmoGoodie : public Goodie
			{
			public:
				AmmoGoodie(int startX, int startY, StudentWorld* world)
					: Goodie(IID_AMMO, startX, startY, world) {}

				virtual void doSomething();
			};

#endif // ACTOR_H_