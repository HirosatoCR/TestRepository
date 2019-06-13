#pragma once

struct PawnStruct
{
	int Direction;	//�����ɂ�1��-1����

	int LoadGraphNumber;
	bool IsPlayer;
};

class BasePawn
{
protected:
	virtual void Move() =0;
public:
	PawnStruct PawnStatus;
};