#include "Object3D.h"

Object3D::Object3D()
    :hModel_(-1), hitModel_(-1), parent_(nullptr), isDraw_(true), hp_(-1), objectNumber_(-1),
    time_(0.0f), gravity_(0), velocityY_(0.0f)
{
}

Object3D::~Object3D()
{
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
    if (hModel_ > 0 && isDraw_ == true)
    {
        const MATRIX& m = transform_.GetLocalMatrix();
        if (parent_ != nullptr)
        {
            MV1SetMatrix(hModel_, m * parent_->GetTransform().GetLocalMatrix());
        }
        else
        {
            MV1SetMatrix(hModel_, m);
        }
        MV1DrawModel(hModel_);
    }
}

bool Object3D::CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit) const
{
    MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(hitModel_, -1, pos1, pos2);
    if (ret.HitFlag == false)
    {
        return false;
    }
    if (hit != nullptr)
    {
        *hit = ret.HitPosition;
    }
    return true;
}

void Object3D::SetMove(VECTOR3 toPosition)
{
    VECTOR3 toGo = toPosition - transform_.position_;
    VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(transform_.rotation_.y); // 正面
    VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(transform_.rotation_.y); // 右　回転の確認に使用

    if (VDot(front, toGo.Normalize()) >= cos(rotateSpeed_))
    {
        transform_.rotation_.y = atan2f(toGo.x, toGo.z);
    }
    else if (VDot(right, toGo) > 0)
    {
        transform_.rotation_.y += rotateSpeed_;
    }
    else
    {
        transform_.rotation_.y -= rotateSpeed_;
    }
    transform_.position_ += VECTOR3(0, 0, moveSpeed_) * MGetRotY(transform_.rotation_.y);
}
