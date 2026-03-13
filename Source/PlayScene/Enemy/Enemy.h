#pragma once
#include "../../Data.h"

namespace Enemy
{
	enum E_SORT {
		STONE,
		TOUHU,
		MAX_E_SORT
	};

	void Update(); // XVˆ—
	void Release(); // ‰ğ•úˆ—

	void CreateEnemy(Data::ObjectData objectData);
	VECTOR3 GetPlayerPosition();
	void AttackPlayer(int attackPower);
}