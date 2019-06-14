#pragma once
#include "DxLib.h"
#include "pieceType.h"
#include "playerType.h"
#include <vector>
#include <map>
#include <assert.h>

#define ARRAY_FIRSTMAPVALUE(array) (&array[0])		//配列の先頭の値を取得するマクロ

#define PLAYER_LINE 0		//プレイヤーのキングなどが並ぶ列の値
#define PLAYER_PAWN_LINE 1	//プレイヤーのポーンが並ぶ列の値

#define ENEMY_LINE 7		//NPCのキングなどが並ぶ列の値
#define ENEMY_PAWN_LINE 6	//NPCのポーンが並ぶ列の値

#define KING_POS 2
#define QUEEN_POS 1
#define ROOK_POS 0
#define BISHOP_POS 3

#define WINDOW_SIZE_X 1920
#define WINDOW_SIZE_Y 1080	//画面の解像度
#define COLOR_BIT_NUM 32	//カラービット数

#define COLOR_MAX_VALUE 255	//色のマックスの時の値

#define MAP_VALUE_CANMOVE 1

//マスの大きさやフィールドの大きさ
int const BoxSize = 200;
int const FieldWidth = 1600;
int const FieldHeight = 800;

//縦横のマス目の個数
int const RectHorizontalNumber = FieldWidth / BoxSize;
int const RectVerticalNumber = FieldHeight / BoxSize;

//駒の情報
struct PieceInfo
{
	PieceType Piece;
	PlayerType Player;
};

//初期の駒配置を2次元配列で設定
PieceInfo const InitialPiecePlacementMap[RectVerticalNumber][RectHorizontalNumber] = 
{
	{ {PieceType::Rook, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::Rook, PlayerType::Enemy}},
	{ {PieceType::King, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::King, PlayerType::Enemy}},
	{ {PieceType::Queen, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::Queen, PlayerType::Enemy}},
	{ {PieceType::Bishop, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::Bishop, PlayerType::Enemy}}
};

namespace map
{
	//画像の情報の構造体、黒と白のそれぞれの駒、背景の画像のintがある
	struct GraphsAdressLists
	{
		int WhitePawn;
		int WhiteKing;
		int WhiteRook;
		int WhiteBishop;
		int WhiteKnight;
		int WhiteQueen;

		int BlackPawn;
		int BlackKing;
		int BlackRook;
		int BlackBishop;
		int BlackKnight;
		int BlackQueen;
		int BackGround;
	};

	//マス目の座標を構造体にしたもの
	struct RectTransform
	{
		int RectPositionX;
		int RectPositionY;
	};

	//各駒の情報、移動情報の先頭の値と移動情報の配列の長さ
	struct PieceInformation
	{
		RectTransform *PieceMapFirstVelue;	//後述している移動情報の配列の先頭の値が入る
		int PieceMapLength;					//移動情報の配列の長さが入る
	};

	//マス目が持つ情報、座標と画像のハンドル、現在の駒の状態、現在のどのプレイヤーが保有しているかを管理する構造体、YとXがRectTransformと逆なのに注意
	struct FieldRectStruct
	{
		int RectPositionY = 0;
		int RectPositionX = 0;
		PieceType ThisPieceType;	//駒の情報、ポーンがこの座標にある時にはこの値はポーンに変わる
		PlayerType	ThisPlayerType;	//このマスの上に載っている駒の操作主は誰かを表す値
	};

	//マス目の情報を管理するクラス
	class BoxInformationManager
	{

	public:
		//コンストラクタで初期化処理を実行
		BoxInformationManager();

		//駒の移動処理、ターゲットのマス目に現在のマス目の情報を上書きする関数、この関数はNPCのターンでも使用する
		void PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece);

		//指定した行と列からBoxInformationを取得する
		FieldRectStruct GetBoxInformation(int arg_row, int arg_col);

		//駒のタイプから移動情報を取得する関数
		PieceInformation GetPieceMap(PieceType arg_pieceType);

		//デバッグ用関数
		void DebugBoxInformation(int X);

		//現在の駒の個数を取得するための関数
		int GetRemainingPiece(PlayerType arg_playerType);
	private:

		//駒のタイプとフィールド移動情報を紐づけるmap
		std::map<PieceType, PieceInformation> PieceMap;

		//PieceMapの設定
		void InitializeMoveMap();

		//フィールドの形と紐づけされている2次元配列の情報、それぞれどのマスに何の種類の駒が誰に操られているかの情報が格納されている
		FieldRectStruct BoxInformation[RectVerticalNumber][RectHorizontalNumber];

		//BoxInformationの初期化
		void InitializeBoxInformation();

		//各駒の移動情報、可変長変数に変更
		std::vector<RectTransform> PawnMap = {
		{1,0}
		};

		std::vector<RectTransform> KingMap = {
			{-1,1},{0,1}, {1,1},
			{-1,0},  {1,0},
			{-1,-1}, {0,-1}, {1,-1}
		};

		std::vector<RectTransform> QueenMap = {
			{-10,-10},{0,-10}, {10,-10},
			{-10,0}, {10,0},
			{-10,10}, {0,10}, {10,10},
		};

		std::vector<RectTransform> RookMap = {
			{0,-10},
			{-10,0},  {10,0},
			{0,10}
		};

		std::vector<RectTransform> BishopMap = {
			{-10,-10},{10,-10},
			{-10,10},{10,10},
		};
	};

}