#pragma once
#include "BaseController.h"

namespace controller
{
	//プレイヤーの駒の操作を扱うBaseControllerの派生クラス
	class PlayerController : public BaseController
	{
	public:
		//移動できるマス目を塗りつぶす処理、現在の地点から移動情報の値を加算した地点までをそれぞれの座標を1つずつ加算して、移動可能かどうか確かめていく処理を行っていく
		void PieceMoveMapDraw(int piecePositionX, int piecePositionY, map::RectTransform* firstMemValue, int length);

		//マウスのクリックを押した時のみTrueを返す関数
		bool PieceSelectPressed();

		//std::clampのように3つの値を渡すとはじめの値が後の2つの値の間にあるかを判定し、間にない場合は最大値か最小値に置き換える関数
		int InsteadOfClamp(int currentValue, int minValue, int maxValue);

		//マウスをクリックした箇所のFieldRectStructを取得する関数
		void MouseClickGetFieldRectStruct();

		//純粋仮想関数をオーバーライドした関数、ゲームのループ部分で行ってほしい処理をここにすべて記述する
		void ControllerUpdate() override;

		//初期化処理、主にメモリ確保やグラフィックハンドルの初期化を行う関数
		void Initialize(map::BoxInformationManager * arg_box);

	private:
		//プレイヤーが駒を選択しているかを確認するための変数
		bool IsSelectingPiece = false;

		//長押ししているかどうかを確認する変数
		bool InputBool = false;
	};
}
