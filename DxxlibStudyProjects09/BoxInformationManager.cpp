#include "BoxInformationManager.h"



map::BoxInformationManager::BoxInformationManager()
{
	InitializeBoxInformation();		//BoxInformationを初期化
	InitializeMoveMap();			//移動情報の配列を駒の種類に紐づけしている
}

//駒の移動処理、ターゲットのマス目に現在のマス目の情報を上書きする関数、この関数はNPCのターンでも使用する
void map::BoxInformationManager::PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece)
{
	assert(currentPiecePositionX >= 0 && currentPiecePositionY >= 0 && currentPiecePositionY < RectVerticalNumber && currentPiecePositionX < RectHorizontalNumber);
	assert(targetPiecePositionY >= 0 && targetPiecePositionX >= 0 && targetPiecePositionY < RectVerticalNumber && targetPiecePositionX < RectHorizontalNumber);
	if (currentPiece.ThisPieceType == PieceType::Pawn && (currentPiece.ThisPlayerType == PlayerType::Player && targetPiecePositionX == ENEMY_LINE) || (currentPiece.ThisPieceType == PieceType::Pawn &&currentPiece.ThisPlayerType == PlayerType::Enemy && targetPiecePositionX == PLAYER_LINE))
	{
		currentPiece.ThisPieceType = PieceType::Queen;		//ポーンが一番奥まで行ったときに初期化初期化処理
	}
	BoxInformation[targetPiecePositionY][targetPiecePositionX] = { targetPiecePositionY,targetPiecePositionX, currentPiece.ThisPieceType, currentPiece.ThisPlayerType };	//先に目標のマスの情報を変更
	BoxInformation[currentPiecePositionY][currentPiecePositionX] = { currentPiecePositionY, currentPiecePositionX,PieceType::None , PlayerType::NonPlayer };				//前にいたマスの情報を変更
}

//BoxinformationからGetする関数
map::FieldRectStruct map::BoxInformationManager::GetBoxInformation(int arg_row, int arg_col)
{
	assert(arg_row >= 0 && arg_col >= 0 && arg_row < RectVerticalNumber && arg_col < RectHorizontalNumber);
	return BoxInformation[arg_row][arg_col];
}

//駒のタイプから移動情報を取得する関数
map::PieceInformation map::BoxInformationManager::GetPieceMap(PieceType arg_pieceType)
{
	assert(arg_pieceType < PieceType::None);
	return PieceMap[arg_pieceType];
}

//デバッグ用関数
void map::BoxInformationManager::DebugBoxInformation(int X)
{
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			DxLib::DrawFormatString(X + _col * 20, 900 + _row * 50, GetColor(255, 255, 255), "%3d", PawnMap.size());
			//BoxInformation[_row][_col] = { _row, _col,PieceType::None , PlayerType::NonPlayer };
		}
	}
}

//現在の駒の個数を取得するための関数
int map::BoxInformationManager::GetRemainingPiece(PlayerType arg_playerType)
{
	assert(arg_playerType < PlayerType::NonPlayer);
	int _returnValue = 0;
	FieldRectStruct _currentSelectRect;
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_currentSelectRect = BoxInformation[_row][_col];
			if (_currentSelectRect.ThisPlayerType == arg_playerType)
			{
				_returnValue++;
			}
		}
	}
	return _returnValue;
}

//PieceMapの設定
void map::BoxInformationManager::InitializeMoveMap()
{
	PieceInformation _insertThis = { &PawnMap[0], (int)PawnMap.size() };	//(int)付いているのはsize()で返ってきたのがunsigned intだったため無理やりParseするため
	PieceMap.insert(std::make_pair(PieceType::Pawn, _insertThis));

	_insertThis = { &KingMap[0], (int)KingMap.size() };
	PieceMap.insert(std::make_pair(PieceType::King, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(QueenMap), (int)QueenMap.size() };
	PieceMap.insert(std::make_pair(PieceType::Queen, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(RookMap), (int)RookMap.size() };
	PieceMap.insert(std::make_pair(PieceType::Rook, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(BishopMap),(int)BishopMap.size() };
	PieceMap.insert(std::make_pair(PieceType::Bishop, _insertThis));
}

//BoxInformationの初期化
void map::BoxInformationManager::InitializeBoxInformation()
{
	//行と列の値を元にそれぞれの座標とPiecePlacementMapから取得してきた
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			BoxInformation[_row][_col] = { _row, _col, InitialPiecePlacementMap[_row][_col].Piece, InitialPiecePlacementMap[_row][_col].Player };
		}
	}
}
