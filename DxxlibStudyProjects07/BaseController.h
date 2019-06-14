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

		//終了処理、メモリ解放用関数
		void End();

	private:
		//プレイヤーが駒を選択しているかを確認するための変数
		bool IsSelectingPiece = false;

		//長押ししているかどうかを確認する変数
		bool InputBool = false;
	};

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

		//終了処理、主にメモリ解放用の関数
		void End();
	};
}

