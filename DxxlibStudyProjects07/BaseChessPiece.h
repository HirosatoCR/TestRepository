#pragma once

struct PawnStruct
{
	int Direction;	//‚±‚±‚É‚Í1‚©-1‚ð‘ã“ü

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