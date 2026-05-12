#include "Animator.h"
#include <string>
#include <assert.h>
#include "../Library/myDxLib.h"
#include "../Library/Time.h"

Animator::Animator(int hModel)
{
	baseModel_ = hModel;
	fileInfos.clear();
	playSpeed_ = 1.0f;
	blendTime_ = 0.0f;
	blendTimeMax_ = 0.0f;
	isFinished_ = true;
}

Animator::~Animator()
{
	if (prevAnim_.attachID >= 0)
	{
		MV1DetachAnim(baseModel_, prevAnim_.attachID);
	}
	if (currentAnim_.attachID = 0)
	{
		MV1DetachAnim(baseModel_, currentAnim_.attachID);
	}
	for (auto& inf : fileInfos)
	{
		if (inf.second.hModel > 0)
		{
			MV1DeleteModel(inf.second.hModel);
			inf.second.hModel = -1;
		}
	}
}

void Animator::Update()
{
	if (currentAnim_.attachID >= 0)
	{
		const FileInfo& f = fileInfos[currentAnim_.fileID];
		currentAnim_.frame += Time::DeltaTime() * playSpeed_ * f.playSpeed * 30.0f;
		if (currentAnim_.frame >= f.maxFrame)
		{
			if (f.loop)
			{
				currentAnim_.frame -= f.maxFrame;
			}
			else
			{
				currentAnim_.frame = f.maxFrame;
				isFinished_ = true;
			}
		}
		MV1SetAttachAnimTime(baseModel_, currentAnim_.attachID, currentAnim_.frame);
	}
	if (prevAnim_.attachID >= 0)
	{
		blendTime_ += Time::DeltaTime();
		if (blendTime_ >= blendTimeMax_)
		{
			MV1SetAttachAnimBlendRate(baseModel_, currentAnim_.attachID, 1.0f);
			MV1SetAttachAnimBlendRate(baseModel_, prevAnim_.attachID, 0.0f);
			MV1DetachAnim(baseModel_, prevAnim_.attachID);
			prevAnim_.attachID = -1;
			prevAnim_.fileID = -1;
			return;
		}
		const FileInfo& f = fileInfos[prevAnim_.fileID];
		prevAnim_.frame += Time::DeltaTime() * playSpeed_ * f.playSpeed * 30.0f;
		if (prevAnim_.frame >= f.maxFrame)
		{
			if (f.loop == true)
			{
				prevAnim_.frame -= f.maxFrame;
			}
			else
			{
				prevAnim_.frame = f.maxFrame;
			}
		}
		MV1SetAttachAnimTime(baseModel_, prevAnim_.attachID, prevAnim_.frame);

		float rate = blendTime_ / blendTimeMax_;
		MV1SetAttachAnimBlendRate(baseModel_, currentAnim_.attachID, rate);
		MV1SetAttachAnimBlendRate(baseModel_, prevAnim_.attachID, 1.0f - rate);
	}
}

void Animator::AddFile(int id, std::string filename, bool loop, float speed)
{
	FileInfo inf;
	inf.hModel = MV1LoadModel(filename.c_str());
	assert(inf.hModel > 0);
	inf.loop = loop;
	inf.playSpeed = speed;
	if (inf.hModel > 0)
	{
		inf.maxFrame = MV1GetAnimTotalTime(inf.hModel, 0);
	}
	if (fileInfos.count(id) > 0)
	{
		// idが使用されている
		assert(false);
	}
	fileInfos.emplace(id, inf);
}

void Animator::Play(int id, float margeTime)
{
	if (currentAnim_.fileID == id)
	{
		return;
	}
	if (prevAnim_.attachID >= 0)
	{
		MV1DetachAnim(baseModel_, prevAnim_.attachID);
		prevAnim_.attachID = -1;
		prevAnim_.fileID = -1;
	}
	if (currentAnim_.attachID >= 0)
	{
		prevAnim_ = currentAnim_;
		blendTimeMax_ = margeTime;
		blendTime_ = 0.0f;
	}
	else
	{
		blendTimeMax_ = 0.0f;
	}
	currentAnim_.fileID = id;

	if (fileInfos.count(id) > 0)
	{
		currentAnim_.attachID = MV1AttachAnim(baseModel_, 0, fileInfos[id].hModel);
		currentAnim_.frame = 0.0f;
		isFinished_ = false;
		MV1SetAttachAnimTime(baseModel_, currentAnim_.attachID, 0.0f);
		if (prevAnim_.attachID >= 0)
		{
			MV1SetAttachAnimBlendRate(baseModel_, currentAnim_.attachID, 0.0f);
			MV1SetAttachAnimBlendRate(baseModel_, prevAnim_.attachID, 1.0f);
		}
		else
		{
			MV1SetAttachAnimBlendRate(baseModel_, currentAnim_.attachID, 1.0f);
		}
	}
}

void Animator::SetPlaySpeed(float speed)
{
	playSpeed_ = speed;
}

float Animator::GetPlaySpeed()
{
	return playSpeed_;
}

bool Animator::IsFinish()
{
	return isFinished_;
}

int Animator::GetCurrentID()
{
	return currentAnim_.fileID;
}

float Animator::GetCurrentFrame()
{
	return currentAnim_.frame;
}

float Animator::GetMaxFrame()
{
	if (currentAnim_.attachID >= 0)
	{
		return fileInfos[currentAnim_.fileID].maxFrame;
	}
	return 0.0f;
}
