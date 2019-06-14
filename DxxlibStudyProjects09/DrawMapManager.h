#pragma once
#include "BoxInformationManager.h"

namespace map
{
	//DefaultMap��BoxInformation�����ɕ`��������N���X�ADefaultMap�̊Ǘ����s���Ă���
	class DrawMapManager
	{

	public:
		//�R���X�g���N�^�ŃE�B���h�E�̐؂�ւ��Ȃǂ�����
		DrawMapManager();		

		//��̉摜�ƃt�B�[���h��`�ʂ���֐��A�`�ʂɊւ��鏈���͂����ȊO�ł͂���ĂȂ��B
		void DrawMap(BoxInformationManager * box);
		
		//DefaultMap�̑S�v�f��0�ɂ���֐�
		void ResetDefaultMap();

		//��̎�ނƃv���C���[����O���t�B�b�N�n���h�����擾���邽�߂̊֐�
		int GetGraphicHandle(PieceType arg_pieceType, PlayerType arg_playerType);	//���������͎擾��������̎�ނƃv���C���[�͓G��������

		//�C�ӂ̍��W��DefaultMap���̒l��ύX���������Ƃ��̊֐�
		void SetDefaultMap(int arg_row, int arg_col, int value);	//���������͕ς�����positionY�A�ς�����positionX�A�l

		//�C�ӂ̍��W��DefaultMap�̒l���擾���邽�߂̊֐�
		int GetDefaultMap(int arg_row, int arg_col);		//����������positionY�ApositionX

		//�O���t�B�b�N�n���h���̐ݒ��map�ւ̃C���T�[�g���s���֐��A�e�R���g���[���[�̏����������ŌĂ΂Ȃ��ƃ_��
		void InitializeGraphMap();
	private:

		//�ړ��Ɋւ��鏈���̂��߂�2�����z��A���̕ϐ����̂͊e�R���g���[���[�ŋ��ʂł���K�v�͂Ȃ�
		int DefaultMap[RectVerticalNumber][RectHorizontalNumber];

		//��̃^�C�v�ƃv���C���[�̏�񂩂�摜�̃f�[�^��R�Â���map
		std::map<std::pair<PieceType, PlayerType>, int> GraphMap;

		//�摜�̍\����
		GraphsAdressLists GraphsList;

		//BoxInformationManager�̃N���X���擾�A�Ֆʂ̕`��ɕK�v
		BoxInformationManager *BoxManager;

	};
	
}
