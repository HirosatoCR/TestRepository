#include "PlayerController.h"



controller::PlayerController::PlayerController()
{
}


controller::PlayerController::~PlayerController()
{
}

void controller::PlayerController::PieceMoveMapDraw(int piecePositionX, int piecePositionY, RectTransform * firstMemValue, int length)
{
	int _addNum = 0;	//移動出来る箇所の数、0の時にはキャラ選択状態を解除する
	int _distanceX = 0;
	int _distanceY = 0;
	int _count = 0;		//for文をどれだけ回すかを決める変数
	int _addX = 0;
	int _addY = 0;		//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値
	int _upperY = 0;
	int _upperX = 0;	//次に移動可能か確認するBoxInformation配列の番号
	FieldRectStruct nextBoxInfo;
	for (int _arrayNumber = 0; _arrayNumber < length; _arrayNumber++)
	{
		_distanceX = (firstMemValue + _arrayNumber)->RectPositionX;
		_distanceY = (firstMemValue + _arrayNumber)->RectPositionY;	//移動情報の配列に記載された座標

		

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

		_addX = _distanceX / _count;
		_addY = _distanceY / _count;	

		//_countの数だけループが回り、そのつどBoxInfomationのPlayerTypeで確認を行う。PlayerTypeがプレイヤーやNPCの時には次のループに移行する
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			_upperY = piecePositionY + _addY * _moveDis;
			_upperX = piecePositionX + _addX * _moveDis;	

			nextBoxInfo = BoxManager.GetBoxInformation(_upperY, _upperX);

			if (_upperY < RectVerticalNumber && _upperX < RectHorizontalNumber && _upperY >= 0 && _upperX >= 0)
			{
				if (nextBoxInfo.ThisPlayerType != PlayerType::Player)
				{
					DefaultMap[_upperY][_upperX] = MAP_VALUE_CANMOVE;
					_addNum++;

					if (nextBoxInfo.ThisPlayerType == PlayerType::Enemy)
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
	MapManager.DrawMap();
}

bool controller::PlayerController::PieceSelectPressed()
{
	bool _returnBool = false;

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
}

int controller::PlayerController::InsteadOfClamp(int currentValue, int minValue, int maxValue)
{
	int _returnValue = currentValue;

	if (currentValue >= maxValue)
	{
		_returnValue = maxValue - 1;
	}
	else if (currentValue < minValue)
	{
		_returnValue = minValue;
	}

	return _returnValue;
}

void controller::PlayerController::MouseClickGetFieldRectStruct()
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
		FieldRectStruct _clickSelectBoxInfromation = BoxManager.GetBoxInformation(_ClickRectY, _ClickRectX);

		//このif文はキャラを選択していない状態でクリックしたときに流れる
		if (IsSelectingPiece == false)
		{
			//選択したマスのPlayerTypeがPlayerならキャラ選択中になり、選択中のBoxInfomationを示すCurrentSelectFieldRectに選択したマス目の情報が設定される
			if (_clickSelectBoxInfromation.ThisPlayerType == PlayerType::Player)
			{
				IsSelectingPiece = !IsSelectingPiece;
				CurrentSelectRect = _clickSelectBoxInfromation;
			}
		}
		//キャラを選択した状態でマウスをクリックした時に処理が慣れる
		else if (IsSelectingPiece == true)
		{
			//はじめにキャラ選択状態を解除する
			IsSelectingPiece = !IsSelectingPiece;

			//選択したマスのPlayerTypeがPlayerじゃないなら次の処理が流れる
			if (_clickSelectBoxInfromation.ThisPlayerType != PlayerType::Player)
			{
				//なおかつ選択したマスが移動出来る場所であるなら移動の処理にながれる
				if (DefaultMap[_ClickRectY][_ClickRectX] == MAP_VALUE_CANMOVE)
				{
					int _lastNumX = CurrentSelectRect.RectPositionX;
					int _lastNumY = CurrentSelectRect.RectPositionY;	//現在の選択しているマスの座標

					_clickSelectBoxInfromation = BoxManager.GetBoxInformation(_lastNumY, _lastNumX);
					BoxManager.PieceMove(_lastNumX, _lastNumY, _ClickRectX, _ClickRectY, _clickSelectBoxInfromation);	//移動処理

					//PrevPlayer = CurrentGameState;	//PrevPlayerに自分の状態を入れる
					//CurrentGameState = GameState::Confirmation;	//盤面の状態の確認のステートに変更する

				}
			}
		}
	}
}



void controller::PlayerController::ControllerUpdate()
{
	MouseClickGetFieldRectStruct();

	//キャラが選択された場合はこの関数たちも呼ばれるようになる。
	if (IsSelectingPiece == true)
	{
		//ここで現在選択中のマスの情報が持っている駒の種類と紐づいている、移動情報の配列の先頭の値と配列の長さを取得する
		CurrentSelectPieceInformation = BoxManager.GetPieceMap(CurrentSelectRect.ThisPieceType);

		//ここで移動可能なマスを赤くする。引数はそれぞれ現在選択中の座標と、移動情報の配列、配列の長さを渡す。
		PieceMoveMapDraw(CurrentSelectRect.RectPositionX, CurrentSelectRect.RectPositionY, CurrentSelectPieceInformation.PieceMapFirstVelue, CurrentSelectPieceInformation.PieceMapLength);
	}
	else if (IsSelectingPiece == false)
	{
		MapManager.ResetDefaultMap();
	}
}
