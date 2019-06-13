/*
	画像のサイズは200＊200で作れ
	ナイト少し特殊だから今は省く
	2019/0606...途中で障害物があるときに越えることが出来なかったため、移動の情報から作り直し
	2019/06/07...見辛いから見やすいように修正+バグ修正

*/


#include "DxLib.h"
#include "math.h"
#include "pieceType.h"
#include "playerType.h"
#include <vector>
#include <map>
#include <list>
#include <random>
#include <algorithm>

#include "GameState.h"

#define WINDOWS	//今はここでプラットフォーム指定

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

#define COLOR_MAX_VALUE 255	//色のマックスの時の値

#define MAP_VALUE_CANMOVE 1


//#define EnemyPiecesNum 8;

#ifdef WINDOWS		//ウィンドウズの時のマスの大きさやフィールドの大きさ
	int const BoxSize = 200;
	int const FieldWidth = 1600;
	int const FieldHeight = 800;
//#endif // WINDOWS
#elif SmartPhone	//スマホは未実装
	int const BoxSize = 200;
	int const fieldWidth = 1600;
	int const fieldHeight = 800;
#endif //

//#define DEBUG

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

//マス目が持つ情報、座標と画像のハンドル、現在の駒の状態、現在のどのプレイヤーが保有しているかを管理する構造体、YとXがRectTransformと逆なのに注意
struct FieldRectStruct
{
	int RectPositionY = 0;
	int RectPositionX = 0;
	int LoadGraphAdress = 0;	//画像のハンドル
	PieceType ThisPieceType;	//駒の情報、ポーンがこの座標にある時にはこの値はポーンに変わる
	PlayerType ThisPlayerType;	//このマスの上に載っている駒の操作主は誰かを表す値
};

//各駒の情報、移動情報の先頭の値と移動情報の配列の長さ
struct PieceInformation
{
	RectTransform *PieceMapFirstVelue;	//後述している移動情報の配列の先頭の値が入る
	int PieceMapLength;					//移動情報の配列の長さが入る
};

//フィールドの値を示す変数、2次元配列、0の時は移動不可、1の時は移動可能である
int DefaultMap[4][8] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
};

//各駒の移動情報、通常配列
RectTransform PawnMap[] ={
	{1,0}
};

RectTransform KingMap[] ={
	{-1,1},{0,1}, {1,1},
	{-1,0}, {0,0}, {1,0},
	{-1,-1}, {0,-1}, {1,-1}
};

RectTransform QueenMap[]= {
	{-10,-10},{0,-10}, {10,-10},
	{-10,0}, {0,0}, {10,0},
	{-10,10}, {0,10}, {10,10},
};

RectTransform RookMap[] = {
	{0,0},{0,-10}, {0,0},
	{-10,0}, {0,0}, {10,0},
	{0,0}, {0,10}, {0,0}
};

RectTransform BishopMap[] = {
	{-10,-10},{0,0}, {10,-10},
	{0,0}, {0,0}, {0,0},
	{-10,10}, {0,0}, {10,10},
};

//現在のゲームの状態、この値によってUpdate内で働く処理が変わっていく
GameState CurrentGameState = GameState::Title;

//駒のタイプとフィールド移動情報を紐づけるmap
std::map<PieceType, PieceInformation> PieceMap;

//現在選択しているピースの情報、初期値は0,0が入っている
PieceInformation CurrentSelectPieceInformation = {0,0};

//現在選択しているマスの情報、クリックで選択したものがここに設定される
FieldRectStruct CurrentSelectFieldRect = {0,0,0,PieceType::None , PlayerType::NonPlayer };

//フィールドの形と紐づけされている2次元配列の情報、それぞれどのマスに何の種類の駒が誰に操られているかの情報が格納されている
FieldRectStruct BoxInformation[RectVerticalNumber][RectHorizontalNumber];

//駒を選択しているか判定するためのbool
bool IsSelectingPiece = false;

//ひとつ前にプレイしていたプレイヤーの判別
int PrevPlayer;

//入力しているかを判定するためのbool
bool InputBool = false;

//画像のアドレスの読み込みとPieceMapの設定
void InitializeLoadGraphAndPieceMapSetting()
{
#ifdef WINDOWS	//ここでそれぞれの画像のグラフィックハンドルを読み込んでいる
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
#endif // 画像サイズなどの問題からこの画像はWindows用

	//_insertThisにそれぞれの移動情報の配列の先頭の値と配列の長さを入れて、mapにinsertしている
	PieceInformation _insertThis = { ARRAY_FIRSTMAPVALUE(PawnMap), ARRAY_LENGTH(PawnMap) };
	PieceMap.insert(std::make_pair(PieceType::Pawn, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(KingMap), ARRAY_LENGTH(KingMap) };
	PieceMap.insert(std::make_pair(PieceType::King, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(QueenMap), ARRAY_LENGTH(QueenMap)};
	PieceMap.insert(std::make_pair(PieceType::Queen, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(RookMap), ARRAY_LENGTH(RookMap) };
	PieceMap.insert(std::make_pair(PieceType::Rook, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(BishopMap), ARRAY_LENGTH(BishopMap) };
	PieceMap.insert(std::make_pair(PieceType::Bishop, _insertThis));

}

//BoxInformationの初期化
void InitializeBoxInformaiton()
{
	//行と列の値を元にそれぞれの座標と背景の画像のグラフィックハンドル、駒の状態とプレイヤーの状態はなしに設定する
	for (int _row =0; _row<RectVerticalNumber; _row++)
	{
		for (int _col =0; _col < RectHorizontalNumber; _col++)
		{
			BoxInformation[_row][_col] = { _row, _col,GraphsList.BackGround,PieceType::None , PlayerType::NonPlayer};	
		}
	}
}

//ゲーム開始時の駒の配置
void PlacementOfPieces()
{
	//はじめにプレイヤーとNPCのポーンを指定した列にあるBoxInformationに設定していく
	for (int _row =0;_row< RectVerticalNumber;_row++)
	{
		BoxInformation[_row][PLAYER_PAWN_LINE] = {_row, PLAYER_PAWN_LINE,GraphsList.WhitePawn,PieceType::Pawn, PlayerType::Player };

		BoxInformation[_row][ENEMY_PAWN_LINE] = { _row, ENEMY_PAWN_LINE,GraphsList.BlackPawn,PieceType::Pawn, PlayerType::Enemy };
	}

	//指定した列にあるBoxInformationにプレイヤーのキング、クイーン、ルーク、ビショップの設定を行う
	BoxInformation[KING_POS][PLAYER_LINE] = { KING_POS,PLAYER_LINE, GraphsList.WhiteKing,PieceType::King, PlayerType::Player };
	BoxInformation[QUEEN_POS][PLAYER_LINE] = { QUEEN_POS,PLAYER_LINE,GraphsList.WhiteQueen, PieceType::Queen,PlayerType::Player };
	BoxInformation[ROOK_POS][PLAYER_LINE] = { ROOK_POS,PLAYER_LINE,GraphsList.WhiteRook,PieceType::Rook, PlayerType::Player };
	BoxInformation[BISHOP_POS][PLAYER_LINE] = { BISHOP_POS,PLAYER_LINE,GraphsList.WhiteBishop,PieceType::Bishop, PlayerType::Player };

	//指定した列にあるBoxInformationにNPCのキング、クイーン、ルーク、ビショップの設定を行う、この時プレイヤーの配置とは点対称になるように調整する
	int _enemyPosY = RectVerticalNumber - KING_POS -1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE, GraphsList.BlackKing,PieceType::King, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - QUEEN_POS-1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,GraphsList.BlackQueen, PieceType::Queen,PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - ROOK_POS-1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,GraphsList.BlackRook,PieceType::Rook, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - BISHOP_POS-1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,GraphsList.BlackBishop,PieceType::Bishop, PlayerType::Enemy };


}

//駒の画像とフィールドを描写する関数、描写に関する処理はここ以外ではやってない。
void DrawMap()
{
	//行×列の値の分だけfor文を回す、はじめに駒と背景の画像を描写、次に移動可能範囲の赤色を描写、最後にグリッド線を描写する
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			int _leftUpperX = _col * BoxSize;	
			int _leftUpperY = _row * BoxSize;	//この2つの値はマスの左上の座標を示す、この座標を元に画像は右下に描画される
			DrawGraph(_leftUpperX, _leftUpperY, BoxInformation[_row][_col].LoadGraphAdress, FALSE);

			int _rightDownerX = _leftUpperX + BoxSize;
			int _rightDownerY = _leftUpperY + BoxSize;	//この2つの値はマスの右下の座標を示す			

			if (DefaultMap[_row][_col] == MAP_VALUE_CANMOVE)	//DefaultMap[_row][_col]が1、つまり移動可能範囲としてDefaultMapに値が渡されている箇所が赤色になる
			{
				DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, 0, 0), TRUE);
			}
			
			DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), FALSE);
						
		}
	}
}

//DefaultMap内の値を全て0に設定
void ResetDefaultMap()
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

//移動できるマス目を塗りつぶす処理、現在の地点から移動情報の値を加算した地点までをそれぞれの座標を1つずつ加算して、移動可能かどうか確かめていく処理を行っていく
void PieceMoveMapDraw(int piecePositionX, int piecePositionY, RectTransform *firstMemValue, int length)
{
	int _addNum = 0;	//移動出来る箇所の数、0の時にはキャラ選択状態を解除する
	for (int _arrayNumber = 0; _arrayNumber < length; _arrayNumber++)
	{
		int _distanceX = (firstMemValue + _arrayNumber)->RectPositionX;
		int _distanceY = (firstMemValue + _arrayNumber)->RectPositionY;	//移動情報の配列に記載された座標
		
		int _count = 0;	//次のfor文をどれだけ回すかを決める変数

		//_distanceXが0じゃなければ_countに代入、0なら_distanceYが_countに代入する。どちらも0なら移動不能ということで次のループに移行する
		if (_distanceX != 0)
		{
			_count = std::abs(_distanceX);
		}
		else if (_distanceY != 0)
		{
			_count = std::abs(_distanceY);
		}
		if (_count == 0)
		{
			continue;
		}

		int _addX = _distanceX / _count;
		int _addY = _distanceY / _count;	//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値

		//_countの数だけループが回り、そのつどBoxInfomationのPlayerTypeで確認を行う。PlayerTypeがプレイヤーやNPCの時には次のループに移行する
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			int _upperY = piecePositionY + _addY * _moveDis;
			int _upperX = piecePositionX + _addX * _moveDis;	//次に移動可能か確認するBoxInformation配列の番号

			if (_upperY < RectVerticalNumber && _upperX < RectHorizontalNumber && _upperY >= 0 && _upperX >= 0)
			{
				if (BoxInformation[_upperY][_upperX].ThisPlayerType != PlayerType::Player)
				{
					DefaultMap[_upperY][_upperX] = MAP_VALUE_CANMOVE;
					_addNum++;

					if (BoxInformation[_upperY][_upperX].ThisPlayerType == PlayerType::Enemy)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	//_addNumが0の時に動くもの
	if (_addNum == 0)
	{
		IsSelectingPiece = !IsSelectingPiece;
	}
	DrawMap();
}

//マウスのクリックを押した時のみTrueを返す関数
bool PieceSelectPressed()
{
	//返り値になる変数
	bool _returnBool = false;

#ifdef WINDOWS	//ウィンドウズでの処理、マウスに関する処理が記載されている

	int _IsMouseInput = GetMouseInput() & MOUSE_INPUT_LEFT;	//ここでマウスの入力状態の値を取得、そしてマウスの左クリック時に1を、クリックしていないときは0を返すように設定

	//グローバル変数であるInputBoolが押して時にtrue、離した時にfalseになるように設定、これで押し続けてもfalseが返るように設定
	if (_IsMouseInput == 1 && InputBool == false)
	{
		InputBool = true;
		_returnBool = true;
	}
	else if (_IsMouseInput == 0 && InputBool == true)
	{
		InputBool = false;
	}

	return _returnBool;
#endif // WINDOWS

#ifdef SmartPhone	//スマホの処理は一切記載なし
	
#endif // SmartPhone

	
}

//デバッグ用の関数
void DebugDefaultMaps()
{
	
	//DxLib::DrawFormatString(400 , 950, Get__color(255, 255, 255), "%3d", *(&queenMap[0]+8));
	//DxLib::DrawFormatString(400 , 1000, Get__color(255, 255, 255), "%3d", (&queenMap[0]+8)->RectPositionY);
	/*
	for (int i = 0; i < std::extent<decltype(queenMap), 0>::value; i++)
	{
		for (int j = 0; j < std::extent<decltype(queenMap), 1>::value; j++)
		{
			DxLib::DrawFormatString(400 + j * 100, 900 + i * 20, Get__color(255, 255, 255), "%3d", &queenMap[i]);
		}
	}*/

	

	for (int i = 0; i < std::extent<decltype(DefaultMap), 0>::value; i++)
	{
		for (int j = 0; j < std::extent<decltype(DefaultMap), 1>::value; j++)
		{
			//DxLib::DrawFormatString(800 + j * 20, 900 + i * 20, Get__color(255, 255, 255), "%3d", BoxInformation[i][j].ThisPlayerType);
			DxLib::DrawFormatString(400 + j * 20, 900 + i * 20, GetColor(255, 255, 255), "%3d", DefaultMap[i][j]);
		}
	}
}

//駒の移動処理、ターゲットのマス目に現在のマス目の情報を上書きする関数、この関数はNPCのターンでも使用する
void PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY,FieldRectStruct currentPiece)
{
	if (currentPiece.ThisPieceType == PieceType::Pawn && (currentPiece.ThisPlayerType == PlayerType::Player && targetPiecePositionX == ENEMY_LINE) || (currentPiece.ThisPlayerType == PlayerType::Enemy && targetPiecePositionX == PLAYER_LINE))
	{
		currentPiece.ThisPieceType = PieceType::Queen;
	}
	BoxInformation[targetPiecePositionY][targetPiecePositionX] = { targetPiecePositionY,targetPiecePositionX, currentPiece.LoadGraphAdress, currentPiece.ThisPieceType, currentPiece.ThisPlayerType };
	BoxInformation[currentPiecePositionY][currentPiecePositionX] = { currentPiecePositionY, currentPiecePositionX, GraphsList.BackGround,PieceType::None , PlayerType::NonPlayer };

	


	DrawMap();
}

//std::clampのように3つの値を渡すとはじめの値が後の2つの値の間にあるかを判定し、間にない場合は最大値か最小値に置き換える関数
int InsteadOfClamp(int currentValue,int minValue , int maxValue)
{
	int _returnValue = currentValue;

	if (currentValue >= maxValue)
	{
		_returnValue = maxValue-1;
	}
	else if(currentValue < minValue)
	{
		_returnValue = minValue;
	}
	
	return _returnValue;
}

//マウスをクリックした箇所のFieldRectStructを取得する関数
void MouseClickGetFieldRectStruct()
{
	if (PieceSelectPressed() == true)
	{
		int _mouseX = 0;
		int _mouseY = 0;	//クリックした時のマウスの座標
		
		//現在のマウスの座標を取得
		GetMousePoint(&_mouseX, &_mouseY);
		
		//_mouseX = std::clamp(_mouseX, 0, 7);
		//_mouseX = std::clamp(_mouseX, 0, 7);

		int _ClickRectX = _mouseX / BoxSize;
		int _ClickRectY = _mouseY / BoxSize;	//マウスの座標をマス目の大きさで割って、現在マウスが最も近い場所にいるマス目の座標を取得
		
		//クリックした座標が盤面上にあるかどうかを判定
		_ClickRectX = InsteadOfClamp(_ClickRectX, 0, RectHorizontalNumber);
		_ClickRectY = InsteadOfClamp(_ClickRectY, 0, RectVerticalNumber);
						
		//このif文はキャラを選択していない状態でクリックしたときに流れる
		if (IsSelectingPiece == false)
		{
			//選択したマスのPlayerTypeがPlayerならキャラ選択中になり、選択中のBoxInfomationを示すCurrentSelectFieldRectに選択したマス目の情報が設定される
			if (BoxInformation[_ClickRectY][_ClickRectX].ThisPlayerType == PlayerType::Player)
			{
				IsSelectingPiece = !IsSelectingPiece;
				CurrentSelectFieldRect = BoxInformation[_ClickRectY][_ClickRectX];
			}
		}
		//キャラを選択した状態でマウスをクリックした時に処理が慣れる
		else if (IsSelectingPiece == true)
		{
			//はじめにキャラ選択状態を解除する
			IsSelectingPiece = !IsSelectingPiece;

			//選択したマスのPlayerTypeがPlayerじゃないなら次の処理が流れる
			if (BoxInformation[_ClickRectY][_ClickRectX].ThisPlayerType != PlayerType::Player)
			{
				//なおかつ選択したマスが移動出来る場所であるなら移動の処理にながれる
				if (DefaultMap[_ClickRectY][_ClickRectX] == MAP_VALUE_CANMOVE)
				{
					int _lastNumX = CurrentSelectFieldRect.RectPositionX;
					int _lastNumY = CurrentSelectFieldRect.RectPositionY;	//現在の選択しているマスの座標

					PieceMove(_lastNumX, _lastNumY, _ClickRectX, _ClickRectY, BoxInformation[_lastNumY][_lastNumX]);	//移動処理

					PrevPlayer = CurrentGameState;	//PrevPlayerに自分の状態を入れる
					CurrentGameState = GameState::Confirmation;	//盤面の状態の確認のステートに変更する

				}
			}
		}
	}


		
}

//勝敗確認用の関数,現在は駒が全部消えたかどうかで判定中
void ConfirmationWinOrLose()
{
	int _playerNum = 0;
	int _enemyNum = 0;	//プレイヤーとNPCの駒の数

	//行×列のfor文を回して、盤面上のそれぞれの駒がどれだけあるか確認
	for (int _row= 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			PlayerType _thisRectPlayerType = BoxInformation[_row][_col].ThisPlayerType;	//現在見ているマスのPlayerType
			if (_thisRectPlayerType == PlayerType::Player)
			{
				_playerNum++;
			}
			else if(_thisRectPlayerType == PlayerType::Enemy)
			{
				_enemyNum++;
			}
		}
	}	

	//このステートに映る前のGameStateは何かを確認し、そのStateとは違うStateに遷移する
	if (PrevPlayer == GameState::PlayerTurn)
	{
		CurrentGameState = GameState::EnemyTurn;
	}
	else if(PrevPlayer == GameState::EnemyTurn)
	{
		CurrentGameState = GameState::PlayerTurn;
	}
	
	//どちらかの駒の数が0の時に勝敗を決める
	if (_playerNum <= 0)
	{
		CurrentGameState = GameState::Lose;
	}
	if (_enemyNum <= 0)
	{
		CurrentGameState = GameState::Win;
	}
}

//プレイヤーがゲームを進めていくための関数
void GameProgressForPlayer()
{
	//ここでCurrentSelectFieldRectに挿入する。キャラが選択されるまではこの関数しか呼ばれない
	MouseClickGetFieldRectStruct();
	
	//キャラが選択された場合はこの関数たちも呼ばれるようになる。
	if (IsSelectingPiece == true)
	{
		//ここで現在選択中のマスの情報が持っている駒の種類と紐づいている、移動情報の配列の先頭の値と配列の長さを取得する
		CurrentSelectPieceInformation = PieceMap[CurrentSelectFieldRect.ThisPieceType];

		//ここで移動可能なマスを赤くする。引数はそれぞれ現在選択中の座標と、移動情報の配列、配列の長さを渡す。
		PieceMoveMapDraw(CurrentSelectFieldRect.RectPositionX, CurrentSelectFieldRect.RectPositionY, CurrentSelectPieceInformation.PieceMapFirstVelue, CurrentSelectPieceInformation.PieceMapLength);
	}
	else if (IsSelectingPiece == false)
	{
		ResetDefaultMap();
	}
}

//マス目上の駒毎の評価マップに値をつけ、その合計をもとめる関数
int EnemyPieceMapEvalute(int pieceX, int pieceY, RectTransform *firstMemValue, int length)
{
	//返り値、どれだけ評価されたかをここで返り値として渡す
	int _returnEvalutionValue = 0;

	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		int _distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		int _distanceY = (firstMemValue + _arrayNum)->RectPositionY;	//移動情報の配列に記載された座標

		int _count = 0;		//次のfor文をどれだけ回すかを決める変数

		//_distanceXが0じゃなければ_countに代入、0なら_distanceYが_countに代入する。どちらも0なら移動不能ということで次のループに移行する
		if (_distanceX != 0)
		{
			_count = std::abs(_distanceX);
		}
		else if (_distanceY != 0)
		{
			_count = std::abs(_distanceY);
		}
		if (_count == 0)
		{
			continue;
		}
		int _addX = _distanceX / _count;
		int _addY = _distanceY / _count;	//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値

		//_countの数だけループが回り、そのつどBoxInfomationのPlayerTypeで確認を行う。PlayerTypeがプレイヤーやNPCの時には次のループに移行する
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			int _nextY = pieceY + _addY * _moveDis;
			int _nextX = pieceX + _addX * _moveDis;		//次に移動可能か確認するBoxInformation配列の番号

			if (_nextY < RectVerticalNumber &&  _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (BoxInformation[_nextY][_nextX].ThisPlayerType != PlayerType::Enemy)
					{
						_returnEvalutionValue += MAP_VALUE_CANMOVE;		//移動可能な場所である場合はMAP_VALUE_CANMOVEが加算される

						if (BoxInformation[_nextY][_nextX].ThisPlayerType == PlayerType::Player)
						{
							_returnEvalutionValue += MAP_VALUE_CANMOVE;	//プレイヤーを取ることが出来る場合はさらにプラスされる
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	return _returnEvalutionValue;	//最後に評価された値を返す
}

//評価した値を元に動かすマス目を選択する関数
FieldRectStruct EnemyPieceDetermationByEvalution()
{
	std::vector<FieldRectStruct> _enemyPiecesArray;	//盤面上に存在するNPCの駒の情報
	std::vector<int> _enemyEvalutionArray;		//駒の評価値の配列、上の配列と入ってくる順番は同じにすること

	//行×列のfor文を回すことによってBoxInformationを全部検索して、駒の情報を取得する
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			if (BoxInformation[_row][_col].ThisPlayerType == PlayerType::Enemy)
			{
				_enemyPiecesArray.push_back(BoxInformation[_row][_col]);
			}
		}
	}

	//駒の数だけfor文を回し、_enemyPieceArrayと同じ長さの配列を作る
	for (int _arrayNum=0; _arrayNum < _enemyPiecesArray.size(); _arrayNum++)
	{
		//現在見ているNPCの駒の移動情報の配列の先頭の値と配列の長さを取得する
		PieceInformation _currentEnemyPiece = PieceMap[_enemyPiecesArray[_arrayNum].ThisPieceType];

		//EnemyPieceMapEvaluteに値を渡し、計算した評価値を配列に追加する
		_enemyEvalutionArray.push_back(EnemyPieceMapEvalute(_enemyPiecesArray[_arrayNum].RectPositionX, _enemyPiecesArray[_arrayNum].RectPositionY, _currentEnemyPiece.PieceMapFirstVelue, _currentEnemyPiece.PieceMapLength));
	}

	
	int _maxNum = 0;	//評価が最大になった時の配列の番号
	int _maxValue = 0;	//これから回すfor文で使う最大値
	
	//配列を全部検索して、_maxValueを超えた時に_maxNumを更新する
	for (int _arrayNum = 0; _arrayNum < _enemyEvalutionArray.size(); _arrayNum++)
	{
		if (_maxValue < _enemyEvalutionArray[_arrayNum])
		{
			_maxValue = _enemyEvalutionArray[_arrayNum];
			_maxNum = _arrayNum;
		}
	}

	return _enemyPiecesArray[_maxNum];
}

//敵の移動先を計算し、DefaultMapに加算する関数
void EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, RectTransform *firstMemValue, int length)
{
	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		int _distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		int _distanceY = (firstMemValue + _arrayNum)->RectPositionY;//移動情報の配列に記載された座標

		int _count = 0;//次のfor文をどれだけ回すかを決める変数

		//_distanceXが0じゃなければ_countに代入、0なら_distanceYが_countに代入する。どちらも0なら移動不能ということで次のループに移行する
		if (_distanceX != 0)
		{
			_count = std::abs(_distanceX);
		}
		else if (_distanceY != 0)
		{
			_count = std::abs(_distanceY);
		}
		if (_count == 0)
		{
			continue;
		}
		int _addX = _distanceX / _count;
		int _addY = _distanceY / _count;	//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値

		//_countの数だけループが回り、そのつどBoxInfomationのPlayerTypeで確認を行う。PlayerTypeがプレイヤーやNPCの時には次のループに移行する
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			int _nextY = piecePositionY - _addY * _moveDis;
			int _nextX = piecePositionX - _addX * _moveDis;	//次に移動可能か確認するBoxInformation配列の番号

			if (_nextY < RectVerticalNumber && _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (BoxInformation[_nextY][_nextX].ThisPlayerType != PlayerType::Enemy)
					{
						DefaultMap[_nextY][_nextX] = MAP_VALUE_CANMOVE;	//移動可能な場所である場合はMAP_VALUE_CANMOVEが加算される

						if (BoxInformation[_nextY][_nextX].ThisPlayerType == PlayerType::Player)
						{
							DefaultMap[_nextY][_nextX] = MAP_VALUE_CANMOVE *2;		//プレイヤーを取ることが出来る場合はさらにプラスされる
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
}

//DefaultMapから移動先を決定する関数
void EnemyMoveDetermation(FieldRectStruct selectRect)
{
	std::vector<RectTransform> rectTransformArray;	//マス目の座標を配列化するもの
	std::vector<int> valueArray;	//マス目の評価の値を配列化したもの、上の配列と同じ順番で値を入れること

	//行×列のfor文を回すことによって、DefaultMapを全部検索し、値が1以上のものを探して、座標と値を配列化する
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			if (DefaultMap[_row][_col] > 0)
			{
				rectTransformArray.push_back({ _col ,_row });	//評価の値が1以上のマス目の座標
				valueArray.push_back(DefaultMap[_row][_col]);	//評価の値
			}

		}
	}

	//評価の値の中での最大値
	int _maxValue = *std::max_element(valueArray.begin(), valueArray.end());

	//移動先となる座標の配列の番号がこの配列に入る
	int _number = 0;

	while (true)
	{
		//ランダムで評価の配列の番号を指定
		int _arrayNum = GetRand(valueArray.size() -1);
		
		//ランダムで指定した配列の要素と最大値が一緒なら_numberに配列の番号を入れて、breakする
		if (valueArray[_arrayNum] == _maxValue)
		{

			_number = _arrayNum;
			break;
		}
	}

	//移動の関数に値を渡す、渡す値は現在の座標、移動先の座標、選択中のマス目の情報である
	PieceMove(selectRect.RectPositionX, selectRect.RectPositionY, rectTransformArray[_number].RectPositionX, rectTransformArray[_number].RectPositionY, selectRect);
	
}

//敵のゲームの流れを決める関数
void GameProgressForEnemy()
{
	//はじめにDefaultMapを初期化する
	ResetDefaultMap();

	//今回移動させるマス目を取得
	FieldRectStruct currentSelectRect = EnemyPieceDetermationByEvalution();

	//選んだマス目の上にある駒の移動情報の配列の先頭の値と配列の長さを取得
	PieceInformation currentSelectPiece = PieceMap[currentSelectRect.ThisPieceType];

	//移動情報の計算を行い、DefaultMapに値を加算していく
	EnemyMoveMapCalculation(currentSelectRect.RectPositionX, currentSelectRect.RectPositionY, currentSelectPiece.PieceMapFirstVelue, currentSelectPiece.PieceMapLength);

	//マス目の情報を渡し、移動先を決める
	EnemyMoveDetermation(currentSelectRect);
	
	PrevPlayer = CurrentGameState;				//PrevPlayerに自分の状態を入れる
	CurrentGameState = GameState::Confirmation;	//盤面の状態の確認のステートに変更する
}

//初期化を行う関数を集めた関数
void GameInitialize()
{
	//フルスクリーンではなくウィンドウモードに変更
	ChangeWindowMode(TRUE);
	//ウィンドウの縦横などを設定
	SetGraphMode(1920, 1080, 32);
	//DxLibの初期化
	DxLib_Init();

	//配列や駒の配置を初期化
	InitializeLoadGraphAndPieceMapSetting();
	InitializeBoxInformaiton();
	PlacementOfPieces();
}

//ゲームのアップデート処理を行う関数
void GameUpdate()
{
	while (ProcessMessage() == 0)
	{
		//スクリーンの一度クリア
		ClearDrawScreen();
		//ダブルバッファリング
		SetDrawScreen(DX_SCREEN_BACK);

		//ゲームの状態毎に処理を変える
		switch (CurrentGameState)
		{
		case Title:			//現在特に処理なし、すぐにプレイやーにステートを渡す
			CurrentGameState = GameState::PlayerTurn;
			break;
		case PlayerTurn:	//GameProgressForPlayer関数を回す
			GameProgressForPlayer();
			break;
		case EnemyTurn:		//GameProgressForEnemy関数を回す
			GameProgressForEnemy();
			break;
		case Confirmation:	//ゲームの勝敗確認のためにConfirmationWinOrLose関数を回す
			ConfirmationWinOrLose();
			break;
		case Win:			//勝利した時にはYOUWINという文字を表示
			DxLib::DrawFormatString(1000, 900, GetColor(255, 255, 255), "YOU WIN! ");
			break;
		case Lose:			//敗北した時にはYOULOSEという文字を表示
			DxLib::DrawFormatString(1000, 900, GetColor(255, 255, 255), "YOU LOSE! ");
			break;
		case NoneState:		//基本ここには来ないはずなのでエラーを表示
			DxLib::DrawFormatString(1000, 900, GetColor(255, 255, 255), "ERROR");
			break;
		}
		
		//DrwaMapでUpdate処理
		DrawMap();

		#ifdef  DEBUG
			DebugDefaultMaps();
		#endif //  DEBUG




		//DxLib::DrawFormatString(1000, 950, GetColor(255, 255, 255), "MX:%3d MY:%3d", CurrentSelectFieldRect.RectPositionX, CurrentSelectFieldRect.RectPositionY);

		DxLib::ScreenFlip();
	}
}

//Main関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameInitialize();

	GameUpdate();

	DxLib::DxLib_End();
	return 0;
}