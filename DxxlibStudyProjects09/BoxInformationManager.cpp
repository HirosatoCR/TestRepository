#include "BoxInformationManager.h"



map::BoxInformationManager::BoxInformationManager()
{
	InitializeBoxInformation();		//BoxInformation��������
	InitializeMoveMap();			//�ړ����̔z�����̎�ނɕR�Â����Ă���
}

//��̈ړ������A�^�[�Q�b�g�̃}�X�ڂɌ��݂̃}�X�ڂ̏����㏑������֐��A���̊֐���NPC�̃^�[���ł��g�p����
void map::BoxInformationManager::PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece)
{
	assert(currentPiecePositionX >= 0 && currentPiecePositionY >= 0 && currentPiecePositionY < RectVerticalNumber && currentPiecePositionX < RectHorizontalNumber);
	assert(targetPiecePositionY >= 0 && targetPiecePositionX >= 0 && targetPiecePositionY < RectVerticalNumber && targetPiecePositionX < RectHorizontalNumber);
	if (currentPiece.ThisPieceType == PieceType::Pawn && (currentPiece.ThisPlayerType == PlayerType::Player && targetPiecePositionX == ENEMY_LINE) || (currentPiece.ThisPieceType == PieceType::Pawn &&currentPiece.ThisPlayerType == PlayerType::Enemy && targetPiecePositionX == PLAYER_LINE))
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
