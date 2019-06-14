#pragma once
#include "DrawMapManager.h"
#include "math.h"
#include <random>
#include <algorithm>

namespace controller
{
	//駒の操作を行う基底クラス
	class BaseController
	{
	protected:
		//DrawMapManagerの実体を作成、こいつはインスタンスが重要なのでこの形
		map::DrawMapManager MapManager;

		//BoxInformationManagerの実体を作成、こいつはBoxInformationの値が重要なのでこの形
		map::BoxInformationManager *BoxManager;

		//純粋仮想関数、ゲームのループ部分で行ってほしい処理をここにすべて記述する
		virtual void ControllerUpdate() =0;

		//現在選択中のマス目の情報
		map::FieldRectStruct CurrentSelectRect;

		//現在選択中の駒の情報
		map::PieceInformation CurrentSelectPieceInformation;

	public:
		//ターンの切り替えを検知する変数、ここは少しごり押し
		bool TurnChange;
	};
}

