#include "GameProgress.h"


//ゲームの初期化処理、main関数が動き始めた時はじめに流れる処理をまとめている
void progress::GameProgress::GameInitialize()
{
	DxLib_Init();
	PlayerController.Initialize(&CurrentUseBoxInformation);
	EnemyController.Initialize(&CurrentUseBoxInformation);	//各コントローラーにこのクラスが実体化させたBoxInformationのアドレスを渡している
}

void progress::GameProgress::ConfirmationWinOrLose()
{
	int _playerNum = 0;
	int _enemyNum = 0;		//各プレイヤーのの駒の数

	_playerNum = CurrentUseBoxInformation.GetRemainingPiece(PlayerType::Player);
	_enemyNum = CurrentUseBoxInformation.GetRemainingPiece(PlayerType::Enemy);	//ここで各プレイヤーの駒の数を取得

	if (_playerNum == 0)
	{
		SetCurrentGameState(GameState::Lose);
	}
	if (_enemyNum == 0)
	{
		SetCurrentGameState(GameState::Win);
	}
}

//InGameの時のアップデート
void progress::GameProgress::InGameUpdate()
{
	switch (CurrentPlayerType)
	{
	case Player:
		PlayerController.ControllerUpdate();
		if (PlayerController.TurnChange == false)
		{
			SetCurrentPlayerType(PlayerType::Enemy);
		}
		break;
	case Enemy:
		EnemyController.ControllerUpdate();

		SetCurrentPlayerType(PlayerType::Player);
		break;
	case NonPlayer:
		break;
	}
	ConfirmationWinOrLose();
}

//ゲームのループ部分を扱う関数
void progress::GameProgress::GameUpdate()
{
	//メインのループ部分がここに当たる
	while (ProcessMessage() == 0)
	{
		//スクリーンの一度クリア
		ClearDrawScreen();
		//ダブルバッファリング
		SetDrawScreen(DX_SCREEN_BACK);

		switch (CurrentGameState)
		{
		case Title:
			SetCurrentGameState(GameState::InGame);
			break;
		case InGame:
			InGameUpdate();
			break;
		case Win:
			DxLib::DrawFormatString(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), "YOU WIN! ");
			break;
		case Lose:
			DxLib::DrawFormatString(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), "YOU LOSE! ");
			break;
		case NoneState:
			DxLib::DrawFormatString(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), "ERROR");
			break;
		}

		ScreenFlip();
	}
}
