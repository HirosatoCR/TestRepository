#pragma once
#include "DrawMapManager.h"
#include "math.h"
#include <random>
#include <algorithm>

namespace controller
{
	
	class BaseController
	{
	protected:
		PlayerType CurrentPlayerType;

		map::DrawMapManager MapManager;
		map::BoxInformationManager *BoxManager = new map::BoxInformationManager();
		virtual void ControllerUpdate(map::BoxInformationManager* box) =0;

		map::FieldRectStruct CurrentSelectRect;

		map::PieceInformation CurrentSelectPieceInformation;

	public:
		BaseController();
		~BaseController();
		bool TurnChange;
	};

	class PlayerController : public BaseController
	{
	public:

		void PieceMoveMapDraw(int piecePositionX, int piecePositionY, map::RectTransform* firstMemValue, int length);
		bool PieceSelectPressed();
		int InsteadOfClamp(int currentValue, int minValue, int maxValue);
		void MouseClickGetFieldRectStruct();

		void ControllerUpdate(map::BoxInformationManager* box) override;

		void Initialize();

	private:
		bool IsSelectingPiece = false;

		bool InputBool = false;
	};

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

		void ControllerUpdate(map::BoxInformationManager* box) override;

		void Initialize();
	};
}

