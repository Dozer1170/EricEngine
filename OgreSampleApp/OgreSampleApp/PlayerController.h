#pragma once
class Ship;

class PlayerController
{
public:
	PlayerController(Ship *controlledShip);
	~PlayerController(void);

	void Update(float deltaTime);
private:
	Ship *mShip;
};

