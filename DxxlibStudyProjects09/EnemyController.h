#pragma once
#include "BaseController.h"

namespace controller
{
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

	};
}

