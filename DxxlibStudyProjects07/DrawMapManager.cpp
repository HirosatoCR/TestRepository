#include "DrawMapManager.h"





map::BoxInformationManager::BoxInformationManager()
{
	InitializeBoxInformation();
	PlacementOfPieces();
	InitializeMoveMap();
}

//駒の移動処理、ターゲットのマス目に現在のマス目の情報を上書きする関数、この関数はNPCのターンでも使用する
void map::BoxInformationManager::PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece)
{
	if (currentPiece.ThisPieceType == PieceType::Pawn && (currentPiece.ThisPlayerType == PlayerType::Player && targetPiecePositionX == ENEMY_LINE) || (currentPiece.ThisPlayerType == PlayerType::Enemy && targetPiecePositionX == PLAYER_LINE))
	{
		currentPiece.ThisPieceType = PieceType::Queen;
	}
	BoxInformation[targetPiecePositionY][targetPiecePositionX] = { targetPiecePositionY,targetPiecePositionX, currentPiece.ThisPieceType, currentPiece.ThisPlayerType };
	BoxInformation[currentPiecePositionY][currentPiecePositionX] = { currentPiecePositionY, currentPiecePositionX,PieceType::None , PlayerType::NonPlayer };
}

//BoxinformationからGetする関数
FieldRectStruct map::BoxInformationManager::GetBoxInformation(int arg_row, int arg_col)
{
	//例外処理入れろ
	return BoxInformation[arg_row][arg_col];
}

PieceInformation map::BoxInformationManager::GetPieceMap(PieceType arg_pieceType)
{
	return PieceMap[arg_pieceType];
}

void map::BoxInformationManager::InitializeMoveMap()
{
	PieceInformation _insertThis = { ARRAY_FIRSTMAPVALUE(PawnMap), ARRAY_LENGTH(PawnMap) };
	PieceMap.insert(std::make_pair(PieceType::Pawn, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(KingMap), ARRAY_LENGTH(KingMap) };
	PieceMap.insert(std::make_pair(PieceType::King, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(QueenMap), ARRAY_LENGTH(QueenMap) };
	PieceMap.insert(std::make_pair(PieceType::Queen, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(RookMap), ARRAY_LENGTH(RookMap) };
	PieceMap.insert(std::make_pair(PieceType::Rook, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(BishopMap), ARRAY_LENGTH(BishopMap) };
	PieceMap.insert(std::make_pair(PieceType::Bishop, _insertThis));
}

void map::BoxInformationManager::InitializeBoxInformation()
{
	//行と列の値を元にそれぞれの座標と背景の画像のグラフィックハンドル、駒の状態とプレイヤーの状態はなしに設定する
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			BoxInformation[_row][_col] = { _row, _col,PieceType::None , PlayerType::NonPlayer };
		}
	}
}

void map::BoxInformationManager::PlacementOfPieces()
{
	//はじめにプレイヤーとNPCのポーンを指定した列にあるBoxInformationに設定していく
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		BoxInformation[_row][PLAYER_PAWN_LINE] = { _row, PLAYER_PAWN_LINE,PieceType::Pawn, PlayerType::Player };

		BoxInformation[_row][ENEMY_PAWN_LINE] = { _row, ENEMY_PAWN_LINE,PieceType::Pawn, PlayerType::Enemy };
	}

	//指定した列にあるBoxInformationにプレイヤーのキング、クイーン、ルーク、ビショップの設定を行う
	BoxInformation[KING_POS][PLAYER_LINE] = { KING_POS,PLAYER_LINE, PieceType::King, PlayerType::Player };
	BoxInformation[QUEEN_POS][PLAYER_LINE] = { QUEEN_POS,PLAYER_LINE, PieceType::Queen,PlayerType::Player };
	BoxInformation[ROOK_POS][PLAYER_LINE] = { ROOK_POS,PLAYER_LINE,PieceType::Rook, PlayerType::Player };
	BoxInformation[BISHOP_POS][PLAYER_LINE] = { BISHOP_POS,PLAYER_LINE,PieceType::Bishop, PlayerType::Player };

	//指定した列にあるBoxInformationにNPCのキング、クイーン、ルーク、ビショップの設定を行う、この時プレイヤーの配置とは点対称になるように調整する
	int _enemyPosY = RectVerticalNumber - KING_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,PieceType::King, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - QUEEN_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE, PieceType::Queen,PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - ROOK_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,PieceType::Rook, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - BISHOP_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,PieceType::Bishop, PlayerType::Enemy };
}


map::DrawMapManager::DrawMapManager()
{
	//フルスクリーンではなくウィンドウモードに変更
	ChangeWindowMode(TRUE);
	//ウィンドウの縦横などを設定
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, COLOR_BIT_NUM);

}


void map::DrawMapManager::Draw()
{
	ClearDrawScreen();
	//ダブルバッファリング
	SetDrawScreen(DX_SCREEN_BACK);

	
}

//駒の画像とフィールドを描写する関数、描写に関する処理はここ以外ではやってない。
void map::DrawMapManager::DrawMap()
{
	int _leftUpperX = 0;
	int _leftUpperY = 0;//この2つの値はマスの左上の座標を示す、この座標を元に画像は右下に描画される

	int _rightDownerX = 0;
	int _rightDownerY = 0;//この2つの値はマスの右下の座標を示す	

	int _graphicHandle = 0;
	FieldRectStruct _currentConfirmBox;

	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_leftUpperX = _col * BoxSize;
			_leftUpperY = _row * BoxSize;	
			_currentConfirmBox = BoxManager.GetBoxInformation(_row, _col);
			_graphicHandle = GetGraphicHandle(_currentConfirmBox.ThisPieceType, _currentConfirmBox.ThisPlayerType);

			DrawGraph(_leftUpperX, _leftUpperY, _graphicHandle, FALSE);

			_rightDownerX = _leftUpperX + BoxSize;
			_rightDownerY = _leftUpperY + BoxSize;			

			if (DefaultMap[_row][_col] == MAP_VALUE_CANMOVE)	//DefaultMap[_row][_col]が1、つまり移動可能範囲としてDefaultMapに値が渡されている箇所が赤色になる
			{
				DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, 0, 0), TRUE);
			}

			DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), FALSE);

		}
	}
}


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
	DrawMap();
}

int map::DrawMapManager::GetGraphicHandle(PieceType arg_pieceType, PlayerType arg_playerType)
{
	int _returnGraphicHandle = 0;
	//ここも例外処理入れろ
	_returnGraphicHandle = GraphMap[std::make_pair(arg_pieceType, arg_playerType)];
	return _returnGraphicHandle;
}

void map::DrawMapManager::SetDefaultMap(int arg_row, int arg_col, int _value)
{
	//例外処理入れろ
	DefaultMap[arg_row][arg_col] = _value;
}

int map::DrawMapManager::GetDefaultMap(int arg_row, int arg_col)
{
	int _returnValue = 0;
	_returnValue = DefaultMap[arg_row][arg_col];
	return _returnValue;
}

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
