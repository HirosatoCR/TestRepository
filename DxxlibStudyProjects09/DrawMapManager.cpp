#include "DrawMapManager.h"


//�R���X�g���N�^�ŃE�B���h�E�̐؂�ւ��Ȃǂ�����
map::DrawMapManager::DrawMapManager()
{
	//�t���X�N���[���ł͂Ȃ��E�B���h�E���[�h�ɕύX
	ChangeWindowMode(TRUE);
	//�E�B���h�E�̏c���Ȃǂ�ݒ�
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, COLOR_BIT_NUM);


}

//��̉摜�ƃt�B�[���h��`�ʂ���֐��A�`�ʂɊւ��鏈���͂����ȊO�ł͂���ĂȂ��B
void map::DrawMapManager::DrawMap(map::BoxInformationManager * box)
{
	BoxManager = box;
	int _leftUpperX = 0;
	int _leftUpperY = 0;//����2�̒l�̓}�X�̍���̍��W�������A���̍��W�����ɉ摜�͉E���ɕ`�悳���

	int _rightDownerX = 0;
	int _rightDownerY = 0;//����2�̒l�̓}�X�̉E���̍��W������	

	int _graphicHandle = 0;					//���ɕ`�ʂ���O���t�B�b�N�n���h��
	FieldRectStruct _currentConfirmBox;		//���݂̊m�F���̃}�X

	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_leftUpperX = _col * BoxSize;
			_leftUpperY = _row * BoxSize;	
			_currentConfirmBox = BoxManager->GetBoxInformation(_row, _col);
			_graphicHandle = GetGraphicHandle(_currentConfirmBox.ThisPieceType, _currentConfirmBox.ThisPlayerType);

			DrawGraph(_leftUpperX, _leftUpperY, _graphicHandle, FALSE);		//�����ŉ摜��`��

			_rightDownerX = _leftUpperX + BoxSize;
			_rightDownerY = _leftUpperY + BoxSize;			

			if (DefaultMap[_row][_col] == MAP_VALUE_CANMOVE)	//DefaultMap[_row][_col]��1�A�܂�ړ��\�͈͂Ƃ���DefaultMap�ɒl���n����Ă���ӏ����ԐF�ɂȂ�
			{
				DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, 0, 0), TRUE);		//�ړ��\�ȃ}�X��Ԃ��`������
			}

			DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), FALSE);		//�����̓O���b�h����`������

		}
	}
}

//DefaultMap�̑S�v�f��0�ɂ���֐�
void map::DrawMapManager::ResetDefaultMap()
{
	//�s�~���̕�����for������
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			DefaultMap[_row][_col] = 0;
		}
	}
}

//��̎�ނƃv���C���[����O���t�B�b�N�n���h�����擾���邽�߂̊֐�
int map::DrawMapManager::GetGraphicHandle(PieceType arg_pieceType, PlayerType arg_playerType)
{
	assert(arg_pieceType <= PieceType::None && arg_playerType <= PlayerType::NonPlayer);
	int _returnGraphicHandle = 0;
	_returnGraphicHandle = GraphMap[std::make_pair(arg_pieceType, arg_playerType)];
	return _returnGraphicHandle;
}

//�C�ӂ̍��W��DefaultMap���̒l��ύX���������Ƃ��̊֐�
void map::DrawMapManager::SetDefaultMap(int arg_row, int arg_col, int _value)
{
	assert(arg_row >= 0 && arg_col >= 0 && arg_row < RectVerticalNumber && arg_col < RectHorizontalNumber);
	DefaultMap[arg_row][arg_col] = _value;
}

//�C�ӂ̍��W��DefaultMap�̒l���擾���邽�߂̊֐�
int map::DrawMapManager::GetDefaultMap(int arg_row, int arg_col)
{
	assert(arg_row >= 0 && arg_col >= 0 && arg_row < RectVerticalNumber && arg_col < RectHorizontalNumber);
	int _returnValue = 0;
	_returnValue = DefaultMap[arg_row][arg_col];
	return _returnValue;
}

//�O���t�B�b�N�n���h���̐ݒ��map�ւ̃C���T�[�g���s���֐��A�e�R���g���[���[�̏����������ŌĂ΂Ȃ��ƃ_��
void map::DrawMapManager::InitializeGraphMap()
{
	GraphsList =
	{
		LoadGraph("WhitePawn.bmp"),
		LoadGraph("WhiteKing.bmp"),
		LoadGraph("WhiteRook.bmp"),
		LoadGraph("WhiteBishop.bmp"),
		LoadGraph("WhiteKnight.bmp"),
		LoadGraph("WhiteQueen.bmp"),

		LoadGraph("BlackPawn.bmp"),
		LoadGraph("BlackKing.bmp"),
		LoadGraph("BlackRook.bmp"),
		LoadGraph("BlackBishop.bmp"),
		LoadGraph("BlackKnight.bmp"),
		LoadGraph("BlackQueen.bmp"),

		LoadGraph("Background.bmp")
	};

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Pawn, PlayerType::Player), GraphsList.WhitePawn));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Pawn, PlayerType::Enemy), GraphsList.BlackPawn));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::King, PlayerType::Player), GraphsList.WhiteKing));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::King, PlayerType::Enemy), GraphsList.BlackKing));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Queen, PlayerType::Player), GraphsList.WhiteQueen));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Queen, PlayerType::Enemy), GraphsList.BlackQueen));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Rook, PlayerType::Player), GraphsList.WhiteRook));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Rook, PlayerType::Enemy), GraphsList.BlackRook));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Bishop, PlayerType::Player), GraphsList.WhiteBishop));
	GraphMap.insert(std::make_pair(std::make_pair(PieceType::Bishop, PlayerType::Enemy), GraphsList.BlackBishop));

	GraphMap.insert(std::make_pair(std::make_pair(PieceType::None, PlayerType::NonPlayer), GraphsList.BackGround));
}
