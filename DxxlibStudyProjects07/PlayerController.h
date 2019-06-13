#pragma once
#include "BaseController.h"

namespace controller
{
	class PlayerController : public BaseController
	{
	public:
		PlayerController();

		void PieceMoveMapDraw(int piecePositionX, int piecePositionY, RectTransform* firstMemValue, int length);
		bool PieceSelectPressed();
		int InsteadOfClamp(int currentValue, int minValue, int maxValue);
		void MouseClickGetFieldRectStruct();
		void GameProgressForPlayer();

	protected:
		void ControllerUpdate() override;

	private:
		bool IsSelectingPiece = false;

		bool InputBool = false;
	};
}
