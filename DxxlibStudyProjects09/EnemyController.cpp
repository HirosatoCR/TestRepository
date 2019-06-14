#include "EnemyController.h"

//マス目上の駒毎の評価マップに値をつけ、その合計をもとめる関数
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

			if (_nextY < RectVerticalNumber &&  _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					_nextBoxInfo = BoxManager->GetBoxInformation(_nextY, _nextX);
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

//評価した値を元に動かすマス目を選択する関数
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

//敵の移動先を計算し、DefaultMapに加算する関数
void controller::EnemyController::EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, map::RectTransform * firstMemValue, int length)
{
	int _distanceX = 0;
	int _distanceY = 0;	//移動情報の配列に記載された座標
	int _count = 0;		//次のfor文をどれだけ回すかを決める変数
	int _addX = 0;
	int _addY = 0;		//次のfor文の時に1つループが進むごとにどれだけ進むかを求めた値
	int _nextY = 0;
	int _nextX = 0;		//次に移動可能か確認するBoxInformation配列の番号
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
			_nextY = piecePositionY - _addY * _moveDis;
			_nextX = piecePositionX - _addX * _moveDis;

			if (_nextY < RectVerticalNumber && _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					_nextBoxInfo = BoxManager->GetBoxInformation(_nextY, _nextX);

					if (_nextBoxInfo.ThisPlayerType != PlayerType::Enemy)
					{
						MapManager.SetDefaultMap(_nextY, _nextX, MAP_VALUE_CANMOVE);	//移動可能な場所である場合はMAP_VALUE_CANMOVEが加算される

						if (_nextBoxInfo.ThisPlayerType == PlayerType::Player)
						{
							MapManager.SetDefaultMap(_nextY, _nextX, MAP_VALUE_CANMOVE * 2);		//プレイヤーを取ることが出来る場合はさらにプラスされる
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

//純粋仮想関数をオーバーライドした関数、ゲームのループ部分で行ってほしい処理をここにすべて記述する
void controller::EnemyController::ControllerUpdate()
{

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

//初期化処理、主にメモリ確保を扱う関数
void controller::EnemyController::Initialize(map::BoxInformationManager * arg_box)
{
	BoxManager = new map::BoxInformationManager;
	BoxManager = arg_box;				//メモリ確保、引数としてきたBoxInformationManagerがもつ値が重要なので参照を受け取って変数にコピー
	MapManager.InitializeGraphMap();
}
