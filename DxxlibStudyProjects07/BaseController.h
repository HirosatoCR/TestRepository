#pragma once
#include "DrawMapManager.h"
#include "math.h"
#include <random>
#include <algorithm>

namespace controller
{
	
	class BaseController
	{
	protected:
		PlayerType CurrentPlayerType;

		map::DrawMapManager MapManager;
		map::BoxInformationManager *BoxManager = new map::BoxInformationManager();
		virtual void ControllerUpdate(map::BoxInformationManager* box) =0;

		map::FieldRectStruct CurrentSelectRect;

		map::PieceInformation CurrentSelectPieceInformation;

	public:
		BaseController();
		~BaseController();
		bool TurnChange;
	};

	class PlayerController : public BaseController
	{
	public:

		void PieceMoveMapDraw(int piecePositionX, int piecePositionY, map::RectTransform* firstMemValue, int length);
		bool PieceSelectPressed();
		int InsteadOfClamp(int currentValue, int minValue, int maxValue);
		void MouseClickGetFieldRectStruct();

		void ControllerUpdate(map::BoxInformationManager* box) override;

		void Initialize();

	private:
		bool IsSelectingPiece = false;

		bool InputBool = false;
	};

	class EnemyController : public BaseController
	{
	public:
		//マス目上の駒毎の評価マップに値をつけ、その合計をもとめる関数
		int EnemyPieceMapEvalute(int pieceX, int pieceY, map::RectTransform *firstMemValue, int length);

		//評価した値を元に動かすマス目を選択する関数
		map::FieldRectStruct EnemyPieceDetermationByEvalution();

		//敵の移動先を計算し、DefaultMapに加算する関数
		void EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, map::RectTransform *firstMemValue, int length);

		//DefaultMapから移動先を決定する関数
		void EnemyMoveDetermation();

		void ControllerUpdate(map::BoxInformationManager* box) override;

		void Initialize();
	};
}

