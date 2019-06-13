#include "DxLib.h"
#include "pieceType.h"
#include "playerType.h"
#include <vector>
#include <map>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))	//配列の長さを取得するマクロ
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

GraphsAdressLists GraphsList;

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

//現在選択しているピースの情報、初期値は0,0が入っている
//PieceInformation CurrentSelectPieceInformation = { 0,0 };

//現在選択しているマスの情報、クリックで選択したものがここに設定される
//FieldRectStruct CurrentSelectFieldRect = { 0,0,PieceType::None , PlayerType::NonPlayer };

int DefaultMap[RectVerticalNumber][RectHorizontalNumber];

	//各駒の移動情報、通常配列
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

	/*
	//駒のタイプとフィールド移動情報を紐づけるmap
	std::map<PieceType, PieceInformation> PieceMap;

	//駒のタイプとプレイヤーの情報から画像のデータを紐づけるmap
	std::map<std::pair<PieceType, PlayerType>, int> graphList;
	*/

namespace map
{
	//
	class BoxInformationManager
	{
	public:
		BoxInformationManager();

		//駒の移動処理、ターゲットのマス目に現在のマス目の情報を上書きする関数、この関数はNPCのターンでも使用する
		void PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece);

		//指定した行と列からBoxInformationを取得する
		FieldRectStruct GetBoxInformation(int arg_row, int arg_col);

		//駒のタイプ
		PieceInformation GetPieceMap(PieceType arg_pieceType);
	private:

		//駒のタイプとフィールド移動情報を紐づけるmap
		std::map<PieceType, PieceInformation> PieceMap;

		//PieceMapの設定
		void InitializeMoveMap();

		//フィールドの形と紐づけされている2次元配列の情報、それぞれどのマスに何の種類の駒が誰に操られているかの情報が格納されている
		FieldRectStruct BoxInformation[RectVerticalNumber][RectHorizontalNumber];

		//BoxInformationの初期化
		void InitializeBoxInformation();

		//ゲーム開始時の駒の配置、今ごり押しだから後でマップで作成
		void PlacementOfPieces();
	};

	//DefaultMapとBoxInformationを元に描画を扱うクラス
	class DrawMapManager
	{

	public:
		DrawMapManager();

		//描画のアップデート処理
		void Draw();

		//駒の画像とフィールドを描写する関数、描写に関する処理はここ以外ではやってない。
		void DrawMap();
		
		void ResetDefaultMap();

		int GetGraphicHandle(PieceType arg_pieceType, PlayerType arg_playerType);	//入れる引数は取得したい駒の種類とプレイヤーは敵か味方か

		void SetDefaultMap(int arg_row, int arg_col, int value);	//入れる引数は変えたいpositionY、変えたいpositionX、値

		int GetDefaultMap(int arg_row, int arg_col);		//入れる引数はpositionY、positionX

	private:
		void InitializeGraphMap();

		int DefaultMap[RectVerticalNumber][RectHorizontalNumber];

		//駒のタイプとプレイヤーの情報から画像のデータを紐づけるmap
		std::map<std::pair<PieceType, PlayerType>, int> GraphMap;

		BoxInformationManager BoxManager;
	};
	
}
