#include "BaseController.h"




controller::BaseController::BaseController()
{
}

controller::BaseController::~BaseController()
{
	//delete &BoxManager;
}



void controller::PlayerController::PieceMoveMapDraw(int piecePositionX, int piecePositionY, map::RectTransform * firstMemValue, int length)
{
	int _addNum = 0;	//移動出来る箇所の数、0の時にはキャラ選択状態を解除する
	int _distanceX = 0;
	int _distanceY = 0;
	int _count = 0;		//for文をどれだけ回すかを決める変数
	int _addX = 0;
	int _addY = 0;		//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値
	int _upperY = 0;
	int _upperX = 0;	//次に移動可能か確認するBoxInformation配列の番号
	map::FieldRectStruct nextBoxInfo;
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

			nextBoxInfo = BoxManager->GetBoxInformation(_upperY, _upperX);

			if (_upperY < RectVerticalNumber && _upperX < RectHorizontalNumber && _upperY >= 0 && _upperX >= 0)
			{
				if (nextBoxInfo.ThisPlayerType != PlayerType::Player)
				{
					MapManager.SetDefaultMap(_upperY, _upperX, MAP_VALUE_CANMOVE);
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
	//MapManager.DrawMap(BoxManager);
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
		map::FieldRectStruct _clickSelectBoxInfromation = BoxManager->GetBoxInformation(_ClickRectY, _ClickRectX);

		int _mapValue = 0;

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
				_mapValue = MapManager.GetDefaultMap(_ClickRectY, _ClickRectX);
				//なおかつ選択したマスが移動出来る場所であるなら移動の処理にながれる
				if (_mapValue == MAP_VALUE_CANMOVE)
				{
					int _lastNumX = CurrentSelectRect.RectPositionX;
					int _lastNumY = CurrentSelectRect.RectPositionY;	//現在の選択しているマスの座標

					_clickSelectBoxInfromation = BoxManager->GetBoxInformation(_lastNumY, _lastNumX);
					BoxManager->PieceMove(_lastNumX, _lastNumY, _ClickRectX, _ClickRectY, _clickSelectBoxInfromation);	//移動処理

					TurnChange = false;
				}
			}
		}
	}
}

void controller::PlayerController::ControllerUpdate(map::BoxInformationManager * box)
{
	BoxManager = box;
	TurnChange = true;
	MouseClickGetFieldRectStruct();

	//キャラが選択された場合はこの関数たちも呼ばれるようになる。
	if (IsSelectingPiece == true)
	{
		//ここで現在選択中のマスの情報が持っている駒の種類と紐づいている、移動情報の配列の先頭の値と配列の長さを取得する
		CurrentSelectPieceInformation = BoxManager->GetPieceMap(CurrentSelectRect.ThisPieceType);

		//ここで移動可能なマスを赤くする。引数はそれぞれ現在選択中の座標と、移動情報の配列、配列の長さを渡す。
		PieceMoveMapDraw(CurrentSelectRect.RectPositionX, CurrentSelectRect.RectPositionY, CurrentSelectPieceInformation.PieceMapFirstVelue, CurrentSelectPieceInformation.PieceMapLength);
	}
	else if (IsSelectingPiece == false)
	{
		MapManager.ResetDefaultMap();
	}

	MapManager.DrawMap(BoxManager);
}

void controller::PlayerController::Initialize()
{
	MapManager.InitializeGraphMap();
}

int controller::EnemyController::EnemyPieceMapEvalute(int pieceX, int pieceY, map::RectTransform * firstMemValue, int length)
{
	//返り値、どれだけ評価されたかをここで返り値として渡す
	int _returnEvalutionValue = 0;

	int _distanceX = 0;
	int _distanceY = 0;		//移動情報の配列に記載された座標

	int _count = 0;			//次のfor文をどれだけ回すかを決める変数

	int _addX = 0;
	int _addY = 0;			//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値

	int _nextY = 0;
	int _nextX = 0;			//次に移動可能か確認するBoxInformation配列の番号

	map::FieldRectStruct _nextBoxInfo;

	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		_distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		_distanceY = (firstMemValue + _arrayNum)->RectPositionY;

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
			_nextY = pieceY + _addY * _moveDis;
			_nextX = pieceX + _addX * _moveDis;

			_nextBoxInfo = BoxManager->GetBoxInformation(_nextY, _nextX);

			if (_nextY < RectVerticalNumber &&  _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (_nextBoxInfo.ThisPlayerType != PlayerType::Enemy)
					{
						_returnEvalutionValue += MAP_VALUE_CANMOVE;		//移動可能な場所である場合はMAP_VALUE_CANMOVEが加算される

						if (_nextBoxInfo.ThisPlayerType == PlayerType::Player)
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

map::FieldRectStruct controller::EnemyController::EnemyPieceDetermationByEvalution()
{
	std::vector<map::FieldRectStruct> _enemyPiecesArray;	//盤面上に存在するNPCの駒の情報
	std::vector<int> _enemyEvalutionArray;		//駒の評価値の配列、上の配列と入ってくる順番は同じにすること

	map::FieldRectStruct _confirmationBox;
	//行×列のfor文を回すことによってBoxInformationを全部検索して、駒の情報を取得する
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_confirmationBox = BoxManager->GetBoxInformation(_row, _col);
			if (_confirmationBox.ThisPlayerType == PlayerType::Enemy)
			{
				_enemyPiecesArray.push_back(_confirmationBox);
			}
		}
	}

	//駒の数だけfor文を回し、_enemyPieceArrayと同じ長さの配列を作る
	for (int _arrayNum = 0; _arrayNum < _enemyPiecesArray.size(); _arrayNum++)
	{
		//現在見ているNPCの駒の移動情報の配列の先頭の値と配列の長さを取得する
		map::PieceInformation _currentEnemyPiece = BoxManager->GetPieceMap(_enemyPiecesArray[_arrayNum].ThisPieceType);

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

void controller::EnemyController::EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, map::RectTransform * firstMemValue, int length)
{
	int _distanceX = 0;
	int _distanceY = 0;
	int _count = 0;		//次のfor文をどれだけ回すかを決める変数
	int _addX = 0;
	int _addY = 0;
	int _nextY = 0;
	int _nextX = 0;		//次に移動可能か確認するBoxInformation配列の番号
	map::FieldRectStruct _nextBoxInfo;
	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		_distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		_distanceY = (firstMemValue + _arrayNum)->RectPositionY;//移動情報の配列に記載された座標		

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
		_addY = _distanceY / _count;	//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値

		//_countの数だけループが回り、そのつどBoxInfomationのPlayerTypeで確認を行う。PlayerTypeがプレイヤーやNPCの時には次のループに移行する
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			_nextY = piecePositionY - _addY * _moveDis;
			_nextX = piecePositionX - _addX * _moveDis;	

			_nextBoxInfo = BoxManager->GetBoxInformation(_nextY, _nextX);

			if (_nextY < RectVerticalNumber && _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (_nextBoxInfo.ThisPlayerType != PlayerType::Enemy)
					{
						MapManager.SetDefaultMap(_nextY, _nextX, MAP_VALUE_CANMOVE);	//移動可能な場所である場合はMAP_VALUE_CANMOVEが加算される

						if (_nextBoxInfo.ThisPlayerType == PlayerType::Player)
						{
							MapManager.SetDefaultMap(_nextY, _nextX, MAP_VALUE_CANMOVE*2);		//プレイヤーを取ることが出来る場合はさらにプラスされる
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

void controller::EnemyController::EnemyMoveDetermation()
{
	std::vector<map::RectTransform> rectTransformArray;	//マス目の座標を配列化するもの
	std::vector<int> valueArray;	//マス目の評価の値を配列化したもの、上の配列と同じ順番で値を入れること

	int _mapValue = 0; 
	//行×列のfor文を回すことによって、DefaultMapを全部検索し、値が1以上のものを探して、座標と値を配列化する
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_mapValue = MapManager.GetDefaultMap(_row, _col);
			if (_mapValue > 0)
			{
				rectTransformArray.push_back({ _col ,_row });	//評価の値が1以上のマス目の座標
				valueArray.push_back(_mapValue);	//評価の値
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
		int _arrayNum = GetRand(valueArray.size() - 1);

		//ランダムで指定した配列の要素と最大値が一緒なら_numberに配列の番号を入れて、breakする
		if (valueArray[_arrayNum] == _maxValue)
		{

			_number = _arrayNum;
			break;
		}
	}

	//移動の関数に値を渡す、渡す値は現在の座標、移動先の座標、選択中のマス目の情報である
	BoxManager->PieceMove(CurrentSelectRect.RectPositionX, CurrentSelectRect.RectPositionY, rectTransformArray[_number].RectPositionX, rectTransformArray[_number].RectPositionY, CurrentSelectRect);
}

void controller::EnemyController::ControllerUpdate(map::BoxInformationManager * box)
{
	BoxManager = box;
	//はじめにDefaultMapを初期化する
	MapManager.ResetDefaultMap();

	//今回移動させるマス目を取得
	CurrentSelectRect = EnemyPieceDetermationByEvalution();

	//選んだマス目の上にある駒の移動情報の配列の先頭の値と配列の長さを取得
	CurrentSelectPieceInformation = BoxManager->GetPieceMap(CurrentSelectRect.ThisPieceType);

	//移動情報の計算を行い、DefaultMapに値を加算していく
	EnemyMoveMapCalculation(CurrentSelectRect.RectPositionX, CurrentSelectRect.RectPositionY, CurrentSelectPieceInformation.PieceMapFirstVelue, CurrentSelectPieceInformation.PieceMapLength);

	//マス目の情報を渡し、移動先を決める
	EnemyMoveDetermation();

	MapManager.DrawMap(BoxManager);
}

void controller::EnemyController::Initialize()
{
	MapManager.InitializeGraphMap();
}
