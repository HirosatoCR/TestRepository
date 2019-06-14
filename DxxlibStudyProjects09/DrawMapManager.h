#pragma once
#include "BoxInformationManager.h"

namespace map
{
	//DefaultMapとBoxInformationを元に描画を扱うクラス、DefaultMapの管理も行っている
	class DrawMapManager
	{

	public:
		//コンストラクタでウィンドウの切り替えなどを扱う
		DrawMapManager();		

		//駒の画像とフィールドを描写する関数、描写に関する処理はここ以外ではやってない。
		void DrawMap(BoxInformationManager * box);
		
		//DefaultMapの全要素を0にする関数
		void ResetDefaultMap();

		//駒の種類とプレイヤーからグラフィックハンドルを取得するための関数
		int GetGraphicHandle(PieceType arg_pieceType, PlayerType arg_playerType);	//入れる引数は取得したい駒の種類とプレイヤーは敵か味方か

		//任意の座標のDefaultMap内の値を変更させたいときの関数
		void SetDefaultMap(int arg_row, int arg_col, int value);	//入れる引数は変えたいpositionY、変えたいpositionX、値

		//任意の座標のDefaultMapの値を取得するための関数
		int GetDefaultMap(int arg_row, int arg_col);		//入れる引数はpositionY、positionX

		//グラフィックハンドルの設定とmapへのインサートを行う関数、各コントローラーの初期化処理で呼ばないとダメ
		void InitializeGraphMap();
	private:

		//移動に関する処理のための2次元配列、この変数自体は各コントローラーで共通である必要はない
		int DefaultMap[RectVerticalNumber][RectHorizontalNumber];

		//駒のタイプとプレイヤーの情報から画像のデータを紐づけるmap
		std::map<std::pair<PieceType, PlayerType>, int> GraphMap;

		//画像の構造体
		GraphsAdressLists GraphsList;

		//BoxInformationManagerのクラスを取得、盤面の描画に必要
		BoxInformationManager *BoxManager;

	};
	
}
