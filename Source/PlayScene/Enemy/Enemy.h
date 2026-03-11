#pragma once
#include "../../Data.h"

namespace Enemy
{
	enum E_SORT {
		STONE,
		TOUHU,
		MAX_E_SORT
	};

	void CreateEnemy(Data::ObjectData objectData);
}