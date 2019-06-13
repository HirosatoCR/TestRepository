#pragma once

struct PawnStruct
{
	int Direction;	//ここには1か-1を代入

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