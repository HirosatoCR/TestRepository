#pragma once
#include "BaseController.h"

namespace controller
{
	//NPCの駒の操作を扱うBaseControllerの派生クラス
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

		//純粋仮想関数をオーバーライドした関数、ゲームのループ部分で行ってほしい処理をここにすべて記述する
		void ControllerUpdate() override;

		//初期化処理、主にメモリ確保を扱う関数
		void Initialize(map::BoxInformationManager * arg_box);

	};
}

