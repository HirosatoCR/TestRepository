#include "DrawMapManager.h"

//�R���X�g���N�^�ŏ��������������s
map::BoxInformationManager::BoxInformationManager()
{
	InitializeBoxInformation();		//BoxInformation��������
	InitializeMoveMap();			//�ړ����̔z�����̎�ނɕR�Â����Ă���
}

//��̈ړ������A�^�[�Q�b�g�̃}�X�ڂɌ��݂̃}�X�ڂ̏����㏑������֐��A���̊֐���NPC�̃^�[���ł��g�p����
void map::BoxInformationManager::PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece)
{

	if (currentPiece.ThisPieceType == PieceType::Pawn && (currentPiece.ThisPlayerType == PlayerType::Player && targetPiecePositionX == ENEMY_LINE) || (currentPiece.ThisPlayerType == PlayerType::Enemy && targetPiecePositionX == PLAYER_LINE))
	{
		currentPiece.ThisPieceType = PieceType::Queen;		//�|�[������ԉ��܂ōs�����Ƃ��ɏ���������������
	}
	BoxInformation[targetPiecePositionY][targetPiecePositionX] = { targetPiecePositionY,targetPiecePositionX, currentPiece.ThisPieceType, currentPiece.ThisPlayerType };	//��ɖڕW�̃}�X�̏���ύX
	BoxInformation[currentPiecePositionY][currentPiecePositionX] = { currentPiecePositionY, currentPiecePositionX,PieceType::None , PlayerType::NonPlayer };				//�O�ɂ����}�X�̏���ύX
}

//Boxinformation����Get����֐�
map::FieldRectStruct map::BoxInformationManager::GetBoxInformation(int arg_row, int arg_col)
{
	assert(arg_row >= 0 && arg_col >= 0 && arg_row < RectVerticalNumber && arg_col < RectHorizontalNumber);
	return BoxInformation[arg_row][arg_col];
}

//��̃^�C�v����ړ������擾����֐�
map::PieceInformation map::BoxInformationManager::GetPieceMap(PieceType arg_pieceType)
{
	assert(arg_pieceType < PieceType::None);
	return PieceMap[arg_pieceType];
}

//�f�o�b�O�p�֐�
void map::BoxInformationManager::DebugBoxInformation(int X)
{
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			DxLib::DrawFormatString(X + _col * 20, 900 + _row * 50, GetColor(255, 255, 255), "%3d", PawnMap.size());
			//BoxInformation[_row][_col] = { _row, _col,PieceType::None , PlayerType::NonPlayer };
		}
	}
}

//���݂̋�̌����擾���邽�߂̊֐�
int map::BoxInformationManager::GetRemainingPiece(PlayerType arg_playerType)
{
	assert(arg_playerType < PlayerType::NonPlayer);
	int _returnValue = 0;
	FieldRectStruct _currentSelectRect;
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_currentSelectRect = BoxInformation[_row][_col];
			if (_currentSelectRect.ThisPlayerType == arg_playerType)
			{
				_returnValue++;
			}
		}
	}
	return _returnValue;
}

//PieceMap�̐ݒ�
void map::BoxInformationManager::InitializeMoveMap()
{
	PieceInformation _insertThis = { &PawnMap[0], (int)PawnMap.size() };	//(int)�t���Ă���̂�size()�ŕԂ��Ă����̂�unsigned int���������ߖ������Parse���邽��
	PieceMap.insert(std::make_pair(PieceType::Pawn, _insertThis));

	_insertThis = { &KingMap[0], (int)KingMap.size() };
	PieceMap.insert(std::make_pair(PieceType::King, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(QueenMap), (int)QueenMap.size() };
	PieceMap.insert(std::make_pair(PieceType::Queen, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(RookMap), (int)RookMap.size() };
	PieceMap.insert(std::make_pair(PieceType::Rook, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(BishopMap),(int)BishopMap.size() };
	PieceMap.insert(std::make_pair(PieceType::Bishop, _insertThis));
}

//BoxInformation�̏�����
void map::BoxInformationManager::InitializeBoxInformation()
{
	//�s�Ɨ�̒l�����ɂ��ꂼ��̍��W��PiecePlacementMap����擾���Ă���
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			BoxInformation[_row][_col] = { _row, _col, InitialPiecePlacementMap[_row][_col].Piece, InitialPiecePlacementMap[_row][_col].Player };
		}
	}
}



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
