#pragma once
#include "BaseController.h"
#include "math.h"
#include <random>
#include <algorithm>

namespace controller
{
	class EnemyController :
		public BaseController
	{
	public:
		EnemyController();
	
	public:
		//�}�X�ڏ�̋�̕]���}�b�v�ɒl�����A���̍��v�����Ƃ߂�֐�
		int EnemyPieceMapEvalute(int pieceX, int pieceY, RectTransform *firstMemValue, int length);

		//�]�������l�����ɓ������}�X�ڂ�I������֐�
		FieldRectStruct EnemyPieceDetermationByEvalution();

		//�G�̈ړ�����v�Z���ADefaultMap�ɉ��Z����֐�
		void EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, RectTransform *firstMemValue, int length);

		//DefaultMap����ړ�������肷��֐�
		void EnemyMoveDetermation();
	private:
	protected:
		void ControllerUpdate() override;
	};
}
