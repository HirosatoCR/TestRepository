#include "DrawMapManager.h"


//コンストラクタでウィンドウの切り替えなどを扱う
map::DrawMapManager::DrawMapManager()
{
	//フルスクリーンではなくウィンドウモードに変更
	ChangeWindowMode(TRUE);
	//ウィンドウの縦横などを設定
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, COLOR_BIT_NUM);


}

//駒の画像とフィールドを描写する関数、描写に関する処理はここ以外ではやってない。
void map::DrawMapManager::DrawMap(map::BoxInformationManager * box)
{
	BoxManager = box;
	int _leftUpperX = 0;
	int _leftUpperY = 0;//この2つの値はマスの左上の座標を示す、この座標を元に画像は右下に描画される

	int _rightDownerX = 0;
	int _rightDownerY = 0;//この2つの値はマスの右下の座標を示す	

	int _graphicHandle = 0;					//次に描写するグラフィックハンドル
	FieldRectStruct _currentConfirmBox;		//現在の確認中のマス

	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_leftUpperX = _col * BoxSize;
			_leftUpperY = _row * BoxSize;	
			_currentConfirmBox = BoxManager->GetBoxInformation(_row, _col);
			_graphicHandle = GetGraphicHandle(_currentConfirmBox.ThisPieceType, _currentConfirmBox.ThisPlayerType);

			DrawGraph(_leftUpperX, _leftUpperY, _graphicHandle, FALSE);		//ここで画像を描写

			_rightDownerX = _leftUpperX + BoxSize;
			_rightDownerY = _leftUpperY + BoxSize;			

			if (DefaultMap[_row][_col] == MAP_VALUE_CANMOVE)	//DefaultMap[_row][_col]が1、つまり移動可能範囲としてDefaultMapに値が渡されている箇所が赤色になる
			{
				DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, 0, 0), TRUE);		//移動可能なマスを赤く描く処理
			}

			DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), FALSE);		//ここはグリッド線を描く処理

		}
	}
}

//DefaultMapの全要素を0にする関数
void map::DrawMapManager::ResetDefaultMap()
{
	//行×数の分だけfor文を回す
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			DefaultMap[_row][_col] = 0;
		}
	}
}

//駒の種類とプレイヤーからグラフィックハンドルを取得するための関数
int map::DrawMapManager::GetGraphicHandle(PieceType arg_pieceType, PlayerType arg_playerType)
{
	assert(arg_pieceType <= PieceType::None && arg_playerType <= PlayerType::NonPlayer);
	int _returnGraphicHandle = 0;
	_returnGraphicHandle = GraphMap[std::make_pair(arg_pieceType, arg_playerType)];
	return _returnGraphicHandle;
}

//任意の座標のDefaultMap内の値を変更させたいときの関数
void map::DrawMapManager::SetDefaultMap(int arg_row, int arg_col, int _value)
{
	assert(arg_row >= 0 && arg_col >= 0 && arg_row < RectVerticalNumber && arg_col < RectHorizontalNumber);
	DefaultMap[arg_row][arg_col] = _value;
}

//任意の座標のDefaultMapの値を取得するための関数
int map::DrawMapManager::GetDefaultMap(int arg_row, int arg_col)
{
	assert(arg_row >= 0 && arg_col >= 0 && arg_row < RectVerticalNumber && arg_col < RectHorizontalNumber);
	int _returnValue = 0;
	_returnValue = DefaultMap[arg_row][arg_col];
	return _returnValue;
}

//グラフィックハンドルの設定とmapへのインサートを行う関数、各コントローラーの初期化処理で呼ばないとダメ
void map::DrawMapManager::InitializeGraphMap()
{
	GraphsList =
	{
		LoadGraph("WhitePawn.bmp"),
		LoadGraph("WhiteKing.bmp"),
		LoadGraph("WhiteRook.bmp"),
		LoadGraph("WhiteBishop.bmp"),
		LoadGraph("WhiteKnight.bmp"),
		LoadGraph("WhiteQueen.bmp"),

		LoadGraph("BlackPawn.bmp"),
		LoadGraph("BlackKing.bmp"),
		LoadGraph("BlackRook.bmp"),
		LoadGraph("BlackBishop.bmp"),
		LoadGraph("BlackKnight.bmp"),
		LoadGraph("BlackQueen.bmp"),

		LoadGraph("Background.bmp")
	};

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Pawn, PlayerType::Player), GraphsList.WhitePawn));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Pawn, PlayerType::Enemy), GraphsList.BlackPawn));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::King, PlayerType::Player), GraphsList.WhiteKing));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::King, PlayerType::Enemy), GraphsList.BlackKing));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Queen, PlayerType::Player), GraphsList.WhiteQueen));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Queen, PlayerType::Enemy), GraphsList.BlackQueen));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Rook, PlayerType::Player), GraphsList.WhiteRook));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Rook, PlayerType::Enemy), GraphsList.BlackRook));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Bishop, PlayerType::Player), GraphsList.WhiteBishop));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Bishop, PlayerType::Enemy), GraphsList.BlackBishop));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::None, PlayerType::NonPlayer), GraphsList.BackGround));
}
