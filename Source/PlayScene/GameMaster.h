#pragma once
#include "../../MyLibrary/Object3D.h"

namespace GameMaster
{
	int Init();		// 初期化
	int Update();	// 更新処理
	int Draw();		// 描画処理
	int Release();	// 解放処理

	int GetPlayerPhaseCount(); // プレイヤーの移動フェーズのカウントを取得する
	int AddPhaseCount(); // 移動フェーズのカウントを増やす　player.cppで呼び出す
}
