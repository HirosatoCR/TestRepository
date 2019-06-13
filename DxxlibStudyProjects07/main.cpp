#include "BaseController.h"
#include "GameState.h"


map::BoxInformationManager box;
namespace progress
{
	class GameProgress
	{
	public:
		void GameInitialize();
		void GameUpdate();
		void InGameUpdate();
		void ConfirmationWinOrLose();

		void SetCurrentGameState(GameState arg_nextState) { CurrentGameState = arg_nextState; }
		void SetCurrentPlayerType(PlayerType arg_playerType) { CurrentPlayerType = arg_playerType; }
	private:
		controller::PlayerController PlayerController;
		controller::EnemyController EnemyController;
		GameState CurrentGameState = GameState::Title;
		int PrevPlayer;
		PlayerType CurrentPlayerType;
	};

	void GameProgress::GameInitialize()
	{
		DxLib_Init();
		PlayerController.Initialize();
		EnemyController.Initialize();
		//controller::BaseController a;
	}
	
	void GameProgress::ConfirmationWinOrLose()
	{
		int _playerNum = 0;
		int _enemyNum = 0;
		_playerNum = box.GetRemainingPiece(PlayerType::Player);
		_enemyNum = box.GetRemainingPiece(PlayerType::Enemy);
		if (_playerNum == 0)
		{
			SetCurrentGameState(GameState::Lose);
		}
		if (_enemyNum == 0)
		{
			SetCurrentGameState(GameState::Win);
		}
	}

	void GameProgress::InGameUpdate()
	{
		switch (CurrentPlayerType)
		{
		case Player:
			PlayerController.ControllerUpdate(&box);
			if (PlayerController.TurnChange == false)
			{
				SetCurrentPlayerType(PlayerType::Enemy);
			}
			break;
		case Enemy:
			EnemyController.ControllerUpdate(&box);

			SetCurrentPlayerType(PlayerType::Player);
			break;
		case NonPlayer:
			break;
		}
		ConfirmationWinOrLose();
	}

	

	void GameProgress::GameUpdate()
	{
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
				break;
			case Lose:
				break;
			case NoneState:
				break;
			}

			ScreenFlip();
		}
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	progress::GameProgress thisGameProgress;
	thisGameProgress.GameInitialize();
	thisGameProgress.GameUpdate();
	DxLib::DxLib_End();
	return 0;
}