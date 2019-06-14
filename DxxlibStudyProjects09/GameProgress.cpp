#include "GameProgress.h"


//�Q�[���̏����������Amain�֐��������n�߂����͂��߂ɗ���鏈�����܂Ƃ߂Ă���
void progress::GameProgress::GameInitialize()
{
	DxLib_Init();
	PlayerController.Initialize(&CurrentUseBoxInformation);
	EnemyController.Initialize(&CurrentUseBoxInformation);	//�e�R���g���[���[�ɂ��̃N���X�����̉�������BoxInformation�̃A�h���X��n���Ă���
}

void progress::GameProgress::ConfirmationWinOrLose()
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

//�Q�[���̃��[�v�����������֐�
void progress::GameProgress::GameUpdate()
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
