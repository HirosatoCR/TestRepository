#pragma once
#include "PlayerController.h"
#include "EnemyController.h"
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
}

