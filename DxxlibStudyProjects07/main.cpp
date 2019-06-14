#include "BaseController.h"
#include "GameState.h"



namespace progress
{
	//ゲームの実際の処理をまとめているクラス
	class GameProgress
	{
	public:
		//ゲームの初期化処理、main関数が動き始めた時はじめに流れる処理をまとめている
		void GameInitialize();

		//ゲームのループ部分の関数
		void GameUpdate();

		//ゲームのループ部分の中でのインゲームの部分のループを扱う関数
		void InGameUpdate();

		//ゲーム終了時の処理を扱う関数
		void GameEnd();

		//駒の数から勝敗の確認を行う関数
		void ConfirmationWinOrLose();

		//ゲームのステートを変更する関数
		void SetCurrentGameState(GameState arg_nextState) { CurrentGameState = arg_nextState; }

		//現在のプレイヤーのステートを変更する関数
		void SetCurrentPlayerType(PlayerType arg_playerType) { CurrentPlayerType = arg_playerType; }
	private:
		//各コントローラー
		controller::PlayerController PlayerController;
		controller::EnemyController EnemyController;

		//ゲームの進行ステート
		GameState CurrentGameState = GameState::Title;
		
		//現在のプレイ中のプレイヤーのステート
		PlayerType CurrentPlayerType;
		
		//現在のBoxInformationの実体
		map::BoxInformationManager CurrentUseBoxInformation;
	};

	//ゲームの初期化処理、main関数が動き始めた時はじめに流れる処理をまとめている
	void GameProgress::GameInitialize()
	{
		DxLib_Init();
		PlayerController.Initialize(&CurrentUseBoxInformation);
		EnemyController.Initialize(&CurrentUseBoxInformation);	//各コントローラーにこのクラスが実体化させたBoxInformationのアドレスを渡している
	}
	
	void GameProgress::ConfirmationWinOrLose()
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
	void GameProgress::InGameUpdate()
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

	//ゲーム終了時の処理を扱う関数
	void GameProgress::GameEnd()
	{
		PlayerController.End();
		EnemyController.End();
	}

	
	//ゲームのループ部分を扱う関数
	void GameProgress::GameUpdate()
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
}

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	progress::GameProgress thisGameProgress;
	thisGameProgress.GameInitialize();
	thisGameProgress.GameUpdate();
	
	DxLib::DxLib_End();
	return 0;
}