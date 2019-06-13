#include "DrawMapManager.h"





map::BoxInformationManager::BoxInformationManager()
{
	InitializeBoxInformation();
	PlacementOfPieces();
	InitializeMoveMap();
}

//��̈ړ������A�^�[�Q�b�g�̃}�X�ڂɌ��݂̃}�X�ڂ̏����㏑������֐��A���̊֐���NPC�̃^�[���ł��g�p����
void map::BoxInformationManager::PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece)
{
	if (currentPiece.ThisPieceType == PieceType::Pawn && (currentPiece.ThisPlayerType == PlayerType::Player && targetPiecePositionX == ENEMY_LINE) || (currentPiece.ThisPlayerType == PlayerType::Enemy && targetPiecePositionX == PLAYER_LINE))
	{
		currentPiece.ThisPieceType = PieceType::Queen;
	}
	BoxInformation[targetPiecePositionY][targetPiecePositionX] = { targetPiecePositionY,targetPiecePositionX, currentPiece.ThisPieceType, currentPiece.ThisPlayerType };
	BoxInformation[currentPiecePositionY][currentPiecePositionX] = { currentPiecePositionY, currentPiecePositionX,PieceType::None , PlayerType::NonPlayer };
}

//Boxinformation����Get����֐�
FieldRectStruct map::BoxInformationManager::GetBoxInformation(int arg_row, int arg_col)
{
	//��O���������
	return BoxInformation[arg_row][arg_col];
}

PieceInformation map::BoxInformationManager::GetPieceMap(PieceType arg_pieceType)
{
	return PieceMap[arg_pieceType];
}

void map::BoxInformationManager::InitializeMoveMap()
{
	PieceInformation _insertThis = { ARRAY_FIRSTMAPVALUE(PawnMap), ARRAY_LENGTH(PawnMap) };
	PieceMap.insert(std::make_pair(PieceType::Pawn, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(KingMap), ARRAY_LENGTH(KingMap) };
	PieceMap.insert(std::make_pair(PieceType::King, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(QueenMap), ARRAY_LENGTH(QueenMap) };
	PieceMap.insert(std::make_pair(PieceType::Queen, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(RookMap), ARRAY_LENGTH(RookMap) };
	PieceMap.insert(std::make_pair(PieceType::Rook, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(BishopMap), ARRAY_LENGTH(BishopMap) };
	PieceMap.insert(std::make_pair(PieceType::Bishop, _insertThis));
}

void map::BoxInformationManager::InitializeBoxInformation()
{
	//�s�Ɨ�̒l�����ɂ��ꂼ��̍��W�Ɣw�i�̉摜�̃O���t�B�b�N�n���h���A��̏�Ԃƃv���C���[�̏�Ԃ͂Ȃ��ɐݒ肷��
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			BoxInformation[_row][_col] = { _row, _col,PieceType::None , PlayerType::NonPlayer };
		}
	}
}

void map::BoxInformationManager::PlacementOfPieces()
{
	//�͂��߂Ƀv���C���[��NPC�̃|�[�����w�肵����ɂ���BoxInformation�ɐݒ肵�Ă���
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		BoxInformation[_row][PLAYER_PAWN_LINE] = { _row, PLAYER_PAWN_LINE,PieceType::Pawn, PlayerType::Player };

		BoxInformation[_row][ENEMY_PAWN_LINE] = { _row, ENEMY_PAWN_LINE,PieceType::Pawn, PlayerType::Enemy };
	}

	//�w�肵����ɂ���BoxInformation�Ƀv���C���[�̃L���O�A�N�C�[���A���[�N�A�r�V���b�v�̐ݒ���s��
	BoxInformation[KING_POS][PLAYER_LINE] = { KING_POS,PLAYER_LINE, PieceType::King, PlayerType::Player };
	BoxInformation[QUEEN_POS][PLAYER_LINE] = { QUEEN_POS,PLAYER_LINE, PieceType::Queen,PlayerType::Player };
	BoxInformation[ROOK_POS][PLAYER_LINE] = { ROOK_POS,PLAYER_LINE,PieceType::Rook, PlayerType::Player };
	BoxInformation[BISHOP_POS][PLAYER_LINE] = { BISHOP_POS,PLAYER_LINE,PieceType::Bishop, PlayerType::Player };

	//�w�肵����ɂ���BoxInformation��NPC�̃L���O�A�N�C�[���A���[�N�A�r�V���b�v�̐ݒ���s���A���̎��v���C���[�̔z�u�Ƃ͓_�Ώ̂ɂȂ�悤�ɒ�������
	int _enemyPosY = RectVerticalNumber - KING_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,PieceType::King, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - QUEEN_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE, PieceType::Queen,PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - ROOK_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,PieceType::Rook, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - BISHOP_POS - 1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,PieceType::Bishop, PlayerType::Enemy };
}


map::DrawMapManager::DrawMapManager()
{
	//�t���X�N���[���ł͂Ȃ��E�B���h�E���[�h�ɕύX
	ChangeWindowMode(TRUE);
	//�E�B���h�E�̏c���Ȃǂ�ݒ�
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, COLOR_BIT_NUM);

}


void map::DrawMapManager::Draw()
{
	ClearDrawScreen();
	//�_�u���o�b�t�@�����O
	SetDrawScreen(DX_SCREEN_BACK);

	
}

//��̉摜�ƃt�B�[���h��`�ʂ���֐��A�`�ʂɊւ��鏈���͂����ȊO�ł͂���ĂȂ��B
void map::DrawMapManager::DrawMap()
{
	int _leftUpperX = 0;
	int _leftUpperY = 0;//����2�̒l�̓}�X�̍���̍��W�������A���̍��W�����ɉ摜�͉E���ɕ`�悳���

	int _rightDownerX = 0;
	int _rightDownerY = 0;//����2�̒l�̓}�X�̉E���̍��W������	

	int _graphicHandle = 0;
	FieldRectStruct _currentConfirmBox;

	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_leftUpperX = _col * BoxSize;
			_leftUpperY = _row * BoxSize;	
			_currentConfirmBox = BoxManager.GetBoxInformation(_row, _col);
			_graphicHandle = GetGraphicHandle(_currentConfirmBox.ThisPieceType, _currentConfirmBox.ThisPlayerType);

			DrawGraph(_leftUpperX, _leftUpperY, _graphicHandle, FALSE);

			_rightDownerX = _leftUpperX + BoxSize;
			_rightDownerY = _leftUpperY + BoxSize;			

			if (DefaultMap[_row][_col] == MAP_VALUE_CANMOVE)	//DefaultMap[_row][_col]��1�A�܂�ړ��\�͈͂Ƃ���DefaultMap�ɒl���n����Ă���ӏ����ԐF�ɂȂ�
			{
				DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, 0, 0), TRUE);
			}

			DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), FALSE);

		}
	}
}


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
	DrawMap();
}

int map::DrawMapManager::GetGraphicHandle(PieceType arg_pieceType, PlayerType arg_playerType)
{
	int _returnGraphicHandle = 0;
	//��������O���������
	_returnGraphicHandle = GraphMap[std::make_pair(arg_pieceType, arg_playerType)];
	return _returnGraphicHandle;
}

void map::DrawMapManager::SetDefaultMap(int arg_row, int arg_col, int _value)
{
	//��O���������
	DefaultMap[arg_row][arg_col] = _value;
}

int map::DrawMapManager::GetDefaultMap(int arg_row, int arg_col)
{
	int _returnValue = 0;
	_returnValue = DefaultMap[arg_row][arg_col];
	return _returnValue;
}

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
