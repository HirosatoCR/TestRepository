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
}

