#pragma once
#include "DrawMapManager.h"
#include "math.h"
#include <random>
#include <algorithm>

namespace controller
{
	//��̑�����s�����N���X
	class BaseController
	{
	protected:
		//DrawMapManager�̎��̂��쐬�A�����̓C���X�^���X���d�v�Ȃ̂ł��̌`
		map::DrawMapManager MapManager;

		//BoxInformationManager�̎��̂��쐬�A������BoxInformation�̒l���d�v�Ȃ̂ł��̌`
		map::BoxInformationManager *BoxManager;

		//�������z�֐��A�Q�[���̃��[�v�����ōs���Ăق��������������ɂ��ׂċL�q����
		virtual void ControllerUpdate() =0;

		//���ݑI�𒆂̃}�X�ڂ̏��
		map::FieldRectStruct CurrentSelectRect;

		//���ݑI�𒆂̋�̏��
		map::PieceInformation CurrentSelectPieceInformation;

	public:
		//�^�[���̐؂�ւ������m����ϐ��A�����͏������艟��
		bool TurnChange;
	};

	//�v���C���[�̋�̑��������BaseController�̔h���N���X
	class PlayerController : public BaseController
	{
	public:
		//�ړ��ł���}�X�ڂ�h��Ԃ������A���݂̒n�_����ړ����̒l�����Z�����n�_�܂ł����ꂼ��̍��W��1�����Z���āA�ړ��\���ǂ����m���߂Ă����������s���Ă���
		void PieceMoveMapDraw(int piecePositionX, int piecePositionY, map::RectTransform* firstMemValue, int length);

		//�}�E�X�̃N���b�N�����������̂�True��Ԃ��֐�
		bool PieceSelectPressed();

		//std::clamp�̂悤��3�̒l��n���Ƃ͂��߂̒l�����2�̒l�̊Ԃɂ��邩�𔻒肵�A�ԂɂȂ��ꍇ�͍ő�l���ŏ��l�ɒu��������֐�
		int InsteadOfClamp(int currentValue, int minValue, int maxValue);

		//�}�E�X���N���b�N�����ӏ���FieldRectStruct���擾����֐�
		void MouseClickGetFieldRectStruct();

		//�������z�֐����I�[�o�[���C�h�����֐��A�Q�[���̃��[�v�����ōs���Ăق��������������ɂ��ׂċL�q����
		void ControllerUpdate() override;

		//�����������A��Ƀ������m�ۂ�O���t�B�b�N�n���h���̏��������s���֐�
		void Initialize(map::BoxInformationManager * arg_box);

		//�I�������A����������p�֐�
		void End();

	private:
		//�v���C���[�����I�����Ă��邩���m�F���邽�߂̕ϐ�
		bool IsSelectingPiece = false;

		//���������Ă��邩�ǂ������m�F����ϐ�
		bool InputBool = false;
	};

	//NPC�̋�̑��������BaseController�̔h���N���X
	class EnemyController : public BaseController
	{
	public:
		//�}�X�ڏ�̋�̕]���}�b�v�ɒl�����A���̍��v�����Ƃ߂�֐�
		int EnemyPieceMapEvalute(int pieceX, int pieceY, map::RectTransform *firstMemValue, int length);

		//�]�������l�����ɓ������}�X�ڂ�I������֐�
		map::FieldRectStruct EnemyPieceDetermationByEvalution();

		//�G�̈ړ�����v�Z���ADefaultMap�ɉ��Z����֐�
		void EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, map::RectTransform *firstMemValue, int length);

		//DefaultMap����ړ�������肷��֐�
		void EnemyMoveDetermation();

		//�������z�֐����I�[�o�[���C�h�����֐��A�Q�[���̃��[�v�����ōs���Ăق��������������ɂ��ׂċL�q����
		void ControllerUpdate() override;

		//�����������A��Ƀ������m�ۂ������֐�
		void Initialize(map::BoxInformationManager * arg_box);

		//�I�������A��Ƀ���������p�̊֐�
		void End();
	};
}

