#pragma once
#include "BaseController.h"
#include "math.h"
#include <random>
#include <algorithm>

namespace controller
{
	class EnemyController :
		public BaseController
	{
	public:
		EnemyController();
	
	public:
		//マス目上の駒毎の評価マップに値をつけ、その合計をもとめる関数
		int EnemyPieceMapEvalute(int pieceX, int pieceY, RectTransform *firstMemValue, int length);

		//評価した値を元に動かすマス目を選択する関数
		FieldRectStruct EnemyPieceDetermationByEvalution();

		//敵の移動先を計算し、DefaultMapに加算する関数
		void EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, RectTransform *firstMemValue, int length);

		//DefaultMapから移動先を決定する関数
		void EnemyMoveDetermation();
	private:
	protected:
		void ControllerUpdate() override;
	};
}
