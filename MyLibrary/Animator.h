#pragma once
#include <map>
#include <string>

class Animator {
public:
	Animator(int hModel);
	~Animator();

	void Update();
	void AddFile(int id, std::string filename, bool loop, float speed = 1.0f); // アニメーションファイルの追加
	void Play(int id, float margeTime = 0.2f); // アニメーションの再生
	void SetPlaySpeed(float speed); // アニメーションの再生速度の変更
	float GetPlaySpeed(); // 再生速度を返す
	bool IsFinish(); // true → アニメーションが終わっている
	int GetCurrentID(); // 現在のアニメーションのIDを返す
	float GetCurrentFrame(); // 現在再生しているフレームを返す
	float GetMaxFrame(); // 現在再生中の最大フレームを返す
private:
	int baseModel_;
	struct FileInfo
	{
		int hModel;
		bool loop;
		float maxFrame;
		float playSpeed;
		FileInfo() : hModel(-1), loop(false), maxFrame(1.0f), playSpeed(1.0f) {}
	};
	
	std::map<int, FileInfo> fileInfos;
	
	struct PlayInfo
	{
		int fileID;
		int attachID;
		float frame;
		PlayInfo() : fileID(-1), attachID(-1), frame(0.0f) {}
	};

	PlayInfo currentAnim_; // 現在のアニメーション
	PlayInfo prevAnim_; // 前回のアニメーション
	float blendTime_; // ブレンドのタイマー
	float blendTimeMax_; // ブレンドのタイマーの最大値
	bool isFinished_;
	float playSpeed_;
};