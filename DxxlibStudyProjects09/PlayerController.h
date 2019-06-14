#pragma once
#include "BaseController.h"

namespace controller
{
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

	private:
		//�v���C���[�����I�����Ă��邩���m�F���邽�߂̕ϐ�
		bool IsSelectingPiece = false;

		//���������Ă��邩�ǂ������m�F����ϐ�
		bool InputBool = false;
	};
}
