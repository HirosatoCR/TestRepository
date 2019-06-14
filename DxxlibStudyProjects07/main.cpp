#include "BaseController.h"
#include "GameState.h"



namespace progress
{
	//�Q�[���̎��ۂ̏������܂Ƃ߂Ă���N���X
	class GameProgress
	{
	public:
		//�Q�[���̏����������Amain�֐��������n�߂����͂��߂ɗ���鏈�����܂Ƃ߂Ă���
		void GameInitialize();

		//�Q�[���̃��[�v�����̊֐�
		void GameUpdate();

		//�Q�[���̃��[�v�����̒��ł̃C���Q�[���̕����̃��[�v�������֐�
		void InGameUpdate();

		//�Q�[���I�����̏����������֐�
		void GameEnd();

		//��̐����珟�s�̊m�F���s���֐�
		void ConfirmationWinOrLose();

		//�Q�[���̃X�e�[�g��ύX����֐�
		void SetCurrentGameState(GameState arg_nextState) { CurrentGameState = arg_nextState; }

		//���݂̃v���C���[�̃X�e�[�g��ύX����֐�
		void SetCurrentPlayerType(PlayerType arg_playerType) { CurrentPlayerType = arg_playerType; }
	private:
		//�e�R���g���[���[
		controller::PlayerController PlayerController;
		controller::EnemyController EnemyController;

		//�Q�[���̐i�s�X�e�[�g
		GameState CurrentGameState = GameState::Title;
		
		//���݂̃v���C���̃v���C���[�̃X�e�[�g
		PlayerType CurrentPlayerType;
		
		//���݂�BoxInformation�̎���
		map::BoxInformationManager CurrentUseBoxInformation;
	};

	//�Q�[���̏����������Amain�֐��������n�߂����͂��߂ɗ���鏈�����܂Ƃ߂Ă���
	void GameProgress::GameInitialize()
	{
		DxLib_Init();
		PlayerController.Initialize(&CurrentUseBoxInformation);
		EnemyController.Initialize(&CurrentUseBoxInformation);	//�e�R���g���[���[�ɂ��̃N���X�����̉�������BoxInformation�̃A�h���X��n���Ă���
	}
	
	void GameProgress::ConfirmationWinOrLose()
	{
		int _playerNum = 0;
		int _enemyNum = 0;		//�e�v���C���[�̂̋�̐�

		_playerNum = CurrentUseBoxInformation.GetRemainingPiece(PlayerType::Player);
		_enemyNum = CurrentUseBoxInformation.GetRemainingPiece(PlayerType::Enemy);	//�����Ŋe�v���C���[�̋�̐����擾

		if (_playerNum == 0)
		{
			SetCurrentGameState(GameState::Lose);
		}
		if (_enemyNum == 0)
		{
			SetCurrentGameState(GameState::Win);
		}
	}

	//InGame�̎��̃A�b�v�f�[�g
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

	//�Q�[���I�����̏����������֐�
	void GameProgress::GameEnd()
	{
		PlayerController.End();
		EnemyController.End();
	}

	
	//�Q�[���̃��[�v�����������֐�
	void GameProgress::GameUpdate()
	{
		//���C���̃��[�v�����������ɓ�����
		while (ProcessMessage() == 0)
		{
			//�X�N���[���̈�x�N���A
			ClearDrawScreen();
			//�_�u���o�b�t�@�����O
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

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	progress::GameProgress thisGameProgress;
	thisGameProgress.GameInitialize();
	thisGameProgress.GameUpdate();
	
	DxLib::DxLib_End();
	return 0;
}