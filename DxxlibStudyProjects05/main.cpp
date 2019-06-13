/*
	�摜�̃T�C�Y��200��200�ō��
	�i�C�g�������ꂾ���獡�͏Ȃ�
	2019/0606...�r���ŏ�Q��������Ƃ��ɉz���邱�Ƃ��o���Ȃ��������߁A�ړ��̏�񂩂��蒼��
	2019/06/07...���h�����猩�₷���悤�ɏC��+�o�O�C��

*/


#include "DxLib.h"
#include "math.h"
#include "pieceType.h"
#include "playerType.h"
#include <vector>
#include <map>
#include <list>
#include <random>
#include <algorithm>

#include "GameState.h"

#define WINDOWS	//���͂����Ńv���b�g�t�H�[���w��

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))	//�z��̒������擾����}�N��
#define ARRAY_FIRSTMAPVALUE(array) (&array[0])		//�z��̐擪�̒l���擾����}�N��

#define PLAYER_LINE 0		//�v���C���[�̃L���O�Ȃǂ����ԗ�̒l
#define PLAYER_PAWN_LINE 1	//�v���C���[�̃|�[�������ԗ�̒l

#define ENEMY_LINE 7		//NPC�̃L���O�Ȃǂ����ԗ�̒l
#define ENEMY_PAWN_LINE 6	//NPC�̃|�[�������ԗ�̒l

#define KING_POS 2
#define QUEEN_POS 1
#define ROOK_POS 0
#define BISHOP_POS 3

#define COLOR_MAX_VALUE 255	//�F�̃}�b�N�X�̎��̒l

#define MAP_VALUE_CANMOVE 1


//#define EnemyPiecesNum 8;

#ifdef WINDOWS		//�E�B���h�E�Y�̎��̃}�X�̑傫����t�B�[���h�̑傫��
	int const BoxSize = 200;
	int const FieldWidth = 1600;
	int const FieldHeight = 800;
//#endif // WINDOWS
#elif SmartPhone	//�X�}�z�͖�����
	int const BoxSize = 200;
	int const fieldWidth = 1600;
	int const fieldHeight = 800;
#endif //

//#define DEBUG

//�c���̃}�X�ڂ̌�
int const RectHorizontalNumber = FieldWidth / BoxSize;
int const RectVerticalNumber = FieldHeight / BoxSize;

//�摜�̏��̍\���́A���Ɣ��̂��ꂼ��̋�A�w�i�̉摜��int������
struct GraphsAdressLists
{
	int WhitePawn;
	int WhiteKing;
	int WhiteRook;
	int WhiteBishop;
	int WhiteKnight;
	int WhiteQueen;

	int BlackPawn;
	int BlackKing;
	int BlackRook;
	int BlackBishop;
	int BlackKnight;
	int BlackQueen;

	int BackGround;
};

GraphsAdressLists GraphsList;

//�}�X�ڂ̍��W���\���̂ɂ�������
struct RectTransform
{
	int RectPositionX;
	int RectPositionY;
};

//�}�X�ڂ������A���W�Ɖ摜�̃n���h���A���݂̋�̏�ԁA���݂̂ǂ̃v���C���[���ۗL���Ă��邩���Ǘ�����\���́AY��X��RectTransform�Ƌt�Ȃ̂ɒ���
struct FieldRectStruct
{
	int RectPositionY = 0;
	int RectPositionX = 0;
	int LoadGraphAdress = 0;	//�摜�̃n���h��
	PieceType ThisPieceType;	//��̏��A�|�[�������̍��W�ɂ��鎞�ɂ͂��̒l�̓|�[���ɕς��
	PlayerType ThisPlayerType;	//���̃}�X�̏�ɍڂ��Ă����̑����͒N����\���l
};

//�e��̏��A�ړ����̐擪�̒l�ƈړ����̔z��̒���
struct PieceInformation
{
	RectTransform *PieceMapFirstVelue;	//��q���Ă���ړ����̔z��̐擪�̒l������
	int PieceMapLength;					//�ړ����̔z��̒���������
};

//�t�B�[���h�̒l�������ϐ��A2�����z��A0�̎��͈ړ��s�A1�̎��͈ړ��\�ł���
int DefaultMap[4][8] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
};

//�e��̈ړ����A�ʏ�z��
RectTransform PawnMap[] ={
	{1,0}
};

RectTransform KingMap[] ={
	{-1,1},{0,1}, {1,1},
	{-1,0}, {0,0}, {1,0},
	{-1,-1}, {0,-1}, {1,-1}
};

RectTransform QueenMap[]= {
	{-10,-10},{0,-10}, {10,-10},
	{-10,0}, {0,0}, {10,0},
	{-10,10}, {0,10}, {10,10},
};

RectTransform RookMap[] = {
	{0,0},{0,-10}, {0,0},
	{-10,0}, {0,0}, {10,0},
	{0,0}, {0,10}, {0,0}
};

RectTransform BishopMap[] = {
	{-10,-10},{0,0}, {10,-10},
	{0,0}, {0,0}, {0,0},
	{-10,10}, {0,0}, {10,10},
};

//���݂̃Q�[���̏�ԁA���̒l�ɂ����Update���œ����������ς���Ă���
GameState CurrentGameState = GameState::Title;

//��̃^�C�v�ƃt�B�[���h�ړ�����R�Â���map
std::map<PieceType, PieceInformation> PieceMap;

//���ݑI�����Ă���s�[�X�̏��A�����l��0,0�������Ă���
PieceInformation CurrentSelectPieceInformation = {0,0};

//���ݑI�����Ă���}�X�̏��A�N���b�N�őI���������̂������ɐݒ肳���
FieldRectStruct CurrentSelectFieldRect = {0,0,0,PieceType::None , PlayerType::NonPlayer };

//�t�B�[���h�̌`�ƕR�Â�����Ă���2�����z��̏��A���ꂼ��ǂ̃}�X�ɉ��̎�ނ̋�N�ɑ����Ă��邩�̏�񂪊i�[����Ă���
FieldRectStruct BoxInformation[RectVerticalNumber][RectHorizontalNumber];

//���I�����Ă��邩���肷�邽�߂�bool
bool IsSelectingPiece = false;

//�ЂƂO�Ƀv���C���Ă����v���C���[�̔���
int PrevPlayer;

//���͂��Ă��邩�𔻒肷�邽�߂�bool
bool InputBool = false;

//�摜�̃A�h���X�̓ǂݍ��݂�PieceMap�̐ݒ�
void InitializeLoadGraphAndPieceMapSetting()
{
#ifdef WINDOWS	//�����ł��ꂼ��̉摜�̃O���t�B�b�N�n���h����ǂݍ���ł���
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
#endif // �摜�T�C�Y�Ȃǂ̖�肩�炱�̉摜��Windows�p

	//_insertThis�ɂ��ꂼ��̈ړ����̔z��̐擪�̒l�Ɣz��̒��������āAmap��insert���Ă���
	PieceInformation _insertThis = { ARRAY_FIRSTMAPVALUE(PawnMap), ARRAY_LENGTH(PawnMap) };
	PieceMap.insert(std::make_pair(PieceType::Pawn, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(KingMap), ARRAY_LENGTH(KingMap) };
	PieceMap.insert(std::make_pair(PieceType::King, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(QueenMap), ARRAY_LENGTH(QueenMap)};
	PieceMap.insert(std::make_pair(PieceType::Queen, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(RookMap), ARRAY_LENGTH(RookMap) };
	PieceMap.insert(std::make_pair(PieceType::Rook, _insertThis));

	_insertThis = { ARRAY_FIRSTMAPVALUE(BishopMap), ARRAY_LENGTH(BishopMap) };
	PieceMap.insert(std::make_pair(PieceType::Bishop, _insertThis));

}

//BoxInformation�̏�����
void InitializeBoxInformaiton()
{
	//�s�Ɨ�̒l�����ɂ��ꂼ��̍��W�Ɣw�i�̉摜�̃O���t�B�b�N�n���h���A��̏�Ԃƃv���C���[�̏�Ԃ͂Ȃ��ɐݒ肷��
	for (int _row =0; _row<RectVerticalNumber; _row++)
	{
		for (int _col =0; _col < RectHorizontalNumber; _col++)
		{
			BoxInformation[_row][_col] = { _row, _col,GraphsList.BackGround,PieceType::None , PlayerType::NonPlayer};	
		}
	}
}

//�Q�[���J�n���̋�̔z�u
void PlacementOfPieces()
{
	//�͂��߂Ƀv���C���[��NPC�̃|�[�����w�肵����ɂ���BoxInformation�ɐݒ肵�Ă���
	for (int _row =0;_row< RectVerticalNumber;_row++)
	{
		BoxInformation[_row][PLAYER_PAWN_LINE] = {_row, PLAYER_PAWN_LINE,GraphsList.WhitePawn,PieceType::Pawn, PlayerType::Player };

		BoxInformation[_row][ENEMY_PAWN_LINE] = { _row, ENEMY_PAWN_LINE,GraphsList.BlackPawn,PieceType::Pawn, PlayerType::Enemy };
	}

	//�w�肵����ɂ���BoxInformation�Ƀv���C���[�̃L���O�A�N�C�[���A���[�N�A�r�V���b�v�̐ݒ���s��
	BoxInformation[KING_POS][PLAYER_LINE] = { KING_POS,PLAYER_LINE, GraphsList.WhiteKing,PieceType::King, PlayerType::Player };
	BoxInformation[QUEEN_POS][PLAYER_LINE] = { QUEEN_POS,PLAYER_LINE,GraphsList.WhiteQueen, PieceType::Queen,PlayerType::Player };
	BoxInformation[ROOK_POS][PLAYER_LINE] = { ROOK_POS,PLAYER_LINE,GraphsList.WhiteRook,PieceType::Rook, PlayerType::Player };
	BoxInformation[BISHOP_POS][PLAYER_LINE] = { BISHOP_POS,PLAYER_LINE,GraphsList.WhiteBishop,PieceType::Bishop, PlayerType::Player };

	//�w�肵����ɂ���BoxInformation��NPC�̃L���O�A�N�C�[���A���[�N�A�r�V���b�v�̐ݒ���s���A���̎��v���C���[�̔z�u�Ƃ͓_�Ώ̂ɂȂ�悤�ɒ�������
	int _enemyPosY = RectVerticalNumber - KING_POS -1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE, GraphsList.BlackKing,PieceType::King, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - QUEEN_POS-1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,GraphsList.BlackQueen, PieceType::Queen,PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - ROOK_POS-1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,GraphsList.BlackRook,PieceType::Rook, PlayerType::Enemy };

	_enemyPosY = RectVerticalNumber - BISHOP_POS-1;
	BoxInformation[_enemyPosY][ENEMY_LINE] = { _enemyPosY,ENEMY_LINE,GraphsList.BlackBishop,PieceType::Bishop, PlayerType::Enemy };


}

//��̉摜�ƃt�B�[���h��`�ʂ���֐��A�`�ʂɊւ��鏈���͂����ȊO�ł͂���ĂȂ��B
void DrawMap()
{
	//�s�~��̒l�̕�����for�����񂷁A�͂��߂ɋ�Ɣw�i�̉摜��`�ʁA���Ɉړ��\�͈͂̐ԐF��`�ʁA�Ō�ɃO���b�h����`�ʂ���
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			int _leftUpperX = _col * BoxSize;	
			int _leftUpperY = _row * BoxSize;	//����2�̒l�̓}�X�̍���̍��W�������A���̍��W�����ɉ摜�͉E���ɕ`�悳���
			DrawGraph(_leftUpperX, _leftUpperY, BoxInformation[_row][_col].LoadGraphAdress, FALSE);

			int _rightDownerX = _leftUpperX + BoxSize;
			int _rightDownerY = _leftUpperY + BoxSize;	//����2�̒l�̓}�X�̉E���̍��W������			

			if (DefaultMap[_row][_col] == MAP_VALUE_CANMOVE)	//DefaultMap[_row][_col]��1�A�܂�ړ��\�͈͂Ƃ���DefaultMap�ɒl���n����Ă���ӏ����ԐF�ɂȂ�
			{
				DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, 0, 0), TRUE);
			}
			
			DrawBox(_leftUpperX, _leftUpperY, _rightDownerX, _rightDownerY, GetColor(COLOR_MAX_VALUE, COLOR_MAX_VALUE, COLOR_MAX_VALUE), FALSE);
						
		}
	}
}

//DefaultMap���̒l��S��0�ɐݒ�
void ResetDefaultMap()
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

//�ړ��ł���}�X�ڂ�h��Ԃ������A���݂̒n�_����ړ����̒l�����Z�����n�_�܂ł����ꂼ��̍��W��1�����Z���āA�ړ��\���ǂ����m���߂Ă����������s���Ă���
void PieceMoveMapDraw(int piecePositionX, int piecePositionY, RectTransform *firstMemValue, int length)
{
	int _addNum = 0;	//�ړ��o����ӏ��̐��A0�̎��ɂ̓L�����I����Ԃ���������
	for (int _arrayNumber = 0; _arrayNumber < length; _arrayNumber++)
	{
		int _distanceX = (firstMemValue + _arrayNumber)->RectPositionX;
		int _distanceY = (firstMemValue + _arrayNumber)->RectPositionY;	//�ړ����̔z��ɋL�ڂ��ꂽ���W
		
		int _count = 0;	//����for�����ǂꂾ���񂷂������߂�ϐ�

		//_distanceX��0����Ȃ����_count�ɑ���A0�Ȃ�_distanceY��_count�ɑ������B�ǂ����0�Ȃ�ړ��s�\�Ƃ������ƂŎ��̃��[�v�Ɉڍs����
		if (_distanceX != 0)
		{
			_count = std::abs(_distanceX);
		}
		else if (_distanceY != 0)
		{
			_count = std::abs(_distanceY);
		}
		if (_count == 0)
		{
			continue;
		}

		int _addX = _distanceX / _count;
		int _addY = _distanceY / _count;	//����for���̎���1���[�v���i�ނ��Ƃɂǂꂾ���i�ނ������߂��l

		//_count�̐��������[�v�����A���̂�BoxInfomation��PlayerType�Ŋm�F���s���BPlayerType���v���C���[��NPC�̎��ɂ͎��̃��[�v�Ɉڍs����
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			int _upperY = piecePositionY + _addY * _moveDis;
			int _upperX = piecePositionX + _addX * _moveDis;	//���Ɉړ��\���m�F����BoxInformation�z��̔ԍ�

			if (_upperY < RectVerticalNumber && _upperX < RectHorizontalNumber && _upperY >= 0 && _upperX >= 0)
			{
				if (BoxInformation[_upperY][_upperX].ThisPlayerType != PlayerType::Player)
				{
					DefaultMap[_upperY][_upperX] = MAP_VALUE_CANMOVE;
					_addNum++;

					if (BoxInformation[_upperY][_upperX].ThisPlayerType == PlayerType::Enemy)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	//_addNum��0�̎��ɓ�������
	if (_addNum == 0)
	{
		IsSelectingPiece = !IsSelectingPiece;
	}
	DrawMap();
}

//�}�E�X�̃N���b�N�����������̂�True��Ԃ��֐�
bool PieceSelectPressed()
{
	//�Ԃ�l�ɂȂ�ϐ�
	bool _returnBool = false;

#ifdef WINDOWS	//�E�B���h�E�Y�ł̏����A�}�E�X�Ɋւ��鏈�����L�ڂ���Ă���

	int _IsMouseInput = GetMouseInput() & MOUSE_INPUT_LEFT;	//�����Ń}�E�X�̓��͏�Ԃ̒l���擾�A�����ă}�E�X�̍��N���b�N����1���A�N���b�N���Ă��Ȃ��Ƃ���0��Ԃ��悤�ɐݒ�

	//�O���[�o���ϐ��ł���InputBool�������Ď���true�A����������false�ɂȂ�悤�ɐݒ�A����ŉ��������Ă�false���Ԃ�悤�ɐݒ�
	if (_IsMouseInput == 1 && InputBool == false)
	{
		InputBool = true;
		_returnBool = true;
	}
	else if (_IsMouseInput == 0 && InputBool == true)
	{
		InputBool = false;
	}

	return _returnBool;
#endif // WINDOWS

#ifdef SmartPhone	//�X�}�z�̏����͈�؋L�ڂȂ�
	
#endif // SmartPhone

	
}

//�f�o�b�O�p�̊֐�
void DebugDefaultMaps()
{
	
	//DxLib::DrawFormatString(400 , 950, Get__color(255, 255, 255), "%3d", *(&queenMap[0]+8));
	//DxLib::DrawFormatString(400 , 1000, Get__color(255, 255, 255), "%3d", (&queenMap[0]+8)->RectPositionY);
	/*
	for (int i = 0; i < std::extent<decltype(queenMap), 0>::value; i++)
	{
		for (int j = 0; j < std::extent<decltype(queenMap), 1>::value; j++)
		{
			DxLib::DrawFormatString(400 + j * 100, 900 + i * 20, Get__color(255, 255, 255), "%3d", &queenMap[i]);
		}
	}*/

	

	for (int i = 0; i < std::extent<decltype(DefaultMap), 0>::value; i++)
	{
		for (int j = 0; j < std::extent<decltype(DefaultMap), 1>::value; j++)
		{
			//DxLib::DrawFormatString(800 + j * 20, 900 + i * 20, Get__color(255, 255, 255), "%3d", BoxInformation[i][j].ThisPlayerType);
			DxLib::DrawFormatString(400 + j * 20, 900 + i * 20, GetColor(255, 255, 255), "%3d", DefaultMap[i][j]);
		}
	}
}

//��̈ړ������A�^�[�Q�b�g�̃}�X�ڂɌ��݂̃}�X�ڂ̏����㏑������֐��A���̊֐���NPC�̃^�[���ł��g�p����
void PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY,FieldRectStruct currentPiece)
{
	if (currentPiece.ThisPieceType == PieceType::Pawn && (currentPiece.ThisPlayerType == PlayerType::Player && targetPiecePositionX == ENEMY_LINE) || (currentPiece.ThisPlayerType == PlayerType::Enemy && targetPiecePositionX == PLAYER_LINE))
	{
		currentPiece.ThisPieceType = PieceType::Queen;
	}
	BoxInformation[targetPiecePositionY][targetPiecePositionX] = { targetPiecePositionY,targetPiecePositionX, currentPiece.LoadGraphAdress, currentPiece.ThisPieceType, currentPiece.ThisPlayerType };
	BoxInformation[currentPiecePositionY][currentPiecePositionX] = { currentPiecePositionY, currentPiecePositionX, GraphsList.BackGround,PieceType::None , PlayerType::NonPlayer };

	


	DrawMap();
}

//std::clamp�̂悤��3�̒l��n���Ƃ͂��߂̒l�����2�̒l�̊Ԃɂ��邩�𔻒肵�A�ԂɂȂ��ꍇ�͍ő�l���ŏ��l�ɒu��������֐�
int InsteadOfClamp(int currentValue,int minValue , int maxValue)
{
	int _returnValue = currentValue;

	if (currentValue >= maxValue)
	{
		_returnValue = maxValue-1;
	}
	else if(currentValue < minValue)
	{
		_returnValue = minValue;
	}
	
	return _returnValue;
}

//�}�E�X���N���b�N�����ӏ���FieldRectStruct���擾����֐�
void MouseClickGetFieldRectStruct()
{
	if (PieceSelectPressed() == true)
	{
		int _mouseX = 0;
		int _mouseY = 0;	//�N���b�N�������̃}�E�X�̍��W
		
		//���݂̃}�E�X�̍��W���擾
		GetMousePoint(&_mouseX, &_mouseY);
		
		//_mouseX = std::clamp(_mouseX, 0, 7);
		//_mouseX = std::clamp(_mouseX, 0, 7);

		int _ClickRectX = _mouseX / BoxSize;
		int _ClickRectY = _mouseY / BoxSize;	//�}�E�X�̍��W���}�X�ڂ̑傫���Ŋ����āA���݃}�E�X���ł��߂��ꏊ�ɂ���}�X�ڂ̍��W���擾
		
		//�N���b�N�������W���Ֆʏ�ɂ��邩�ǂ����𔻒�
		_ClickRectX = InsteadOfClamp(_ClickRectX, 0, RectHorizontalNumber);
		_ClickRectY = InsteadOfClamp(_ClickRectY, 0, RectVerticalNumber);
						
		//����if���̓L������I�����Ă��Ȃ���ԂŃN���b�N�����Ƃ��ɗ����
		if (IsSelectingPiece == false)
		{
			//�I�������}�X��PlayerType��Player�Ȃ�L�����I�𒆂ɂȂ�A�I�𒆂�BoxInfomation������CurrentSelectFieldRect�ɑI�������}�X�ڂ̏�񂪐ݒ肳���
			if (BoxInformation[_ClickRectY][_ClickRectX].ThisPlayerType == PlayerType::Player)
			{
				IsSelectingPiece = !IsSelectingPiece;
				CurrentSelectFieldRect = BoxInformation[_ClickRectY][_ClickRectX];
			}
		}
		//�L������I��������ԂŃ}�E�X���N���b�N�������ɏ����������
		else if (IsSelectingPiece == true)
		{
			//�͂��߂ɃL�����I����Ԃ���������
			IsSelectingPiece = !IsSelectingPiece;

			//�I�������}�X��PlayerType��Player����Ȃ��Ȃ玟�̏����������
			if (BoxInformation[_ClickRectY][_ClickRectX].ThisPlayerType != PlayerType::Player)
			{
				//�Ȃ����I�������}�X���ړ��o����ꏊ�ł���Ȃ�ړ��̏����ɂȂ����
				if (DefaultMap[_ClickRectY][_ClickRectX] == MAP_VALUE_CANMOVE)
				{
					int _lastNumX = CurrentSelectFieldRect.RectPositionX;
					int _lastNumY = CurrentSelectFieldRect.RectPositionY;	//���݂̑I�����Ă���}�X�̍��W

					PieceMove(_lastNumX, _lastNumY, _ClickRectX, _ClickRectY, BoxInformation[_lastNumY][_lastNumX]);	//�ړ�����

					PrevPlayer = CurrentGameState;	//PrevPlayer�Ɏ����̏�Ԃ�����
					CurrentGameState = GameState::Confirmation;	//�Ֆʂ̏�Ԃ̊m�F�̃X�e�[�g�ɕύX����

				}
			}
		}
	}


		
}

//���s�m�F�p�̊֐�,���݂͋�S�����������ǂ����Ŕ��蒆
void ConfirmationWinOrLose()
{
	int _playerNum = 0;
	int _enemyNum = 0;	//�v���C���[��NPC�̋�̐�

	//�s�~���for�����񂵂āA�Ֆʏ�̂��ꂼ��̋�ǂꂾ�����邩�m�F
	for (int _row= 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			PlayerType _thisRectPlayerType = BoxInformation[_row][_col].ThisPlayerType;	//���݌��Ă���}�X��PlayerType
			if (_thisRectPlayerType == PlayerType::Player)
			{
				_playerNum++;
			}
			else if(_thisRectPlayerType == PlayerType::Enemy)
			{
				_enemyNum++;
			}
		}
	}	

	//���̃X�e�[�g�ɉf��O��GameState�͉������m�F���A����State�Ƃ͈ႤState�ɑJ�ڂ���
	if (PrevPlayer == GameState::PlayerTurn)
	{
		CurrentGameState = GameState::EnemyTurn;
	}
	else if(PrevPlayer == GameState::EnemyTurn)
	{
		CurrentGameState = GameState::PlayerTurn;
	}
	
	//�ǂ��炩�̋�̐���0�̎��ɏ��s�����߂�
	if (_playerNum <= 0)
	{
		CurrentGameState = GameState::Lose;
	}
	if (_enemyNum <= 0)
	{
		CurrentGameState = GameState::Win;
	}
}

//�v���C���[���Q�[����i�߂Ă������߂̊֐�
void GameProgressForPlayer()
{
	//������CurrentSelectFieldRect�ɑ}������B�L�������I�������܂ł͂��̊֐������Ă΂�Ȃ�
	MouseClickGetFieldRectStruct();
	
	//�L�������I�����ꂽ�ꍇ�͂��̊֐��������Ă΂��悤�ɂȂ�B
	if (IsSelectingPiece == true)
	{
		//�����Ō��ݑI�𒆂̃}�X�̏�񂪎����Ă����̎�ނƕR�Â��Ă���A�ړ����̔z��̐擪�̒l�Ɣz��̒������擾����
		CurrentSelectPieceInformation = PieceMap[CurrentSelectFieldRect.ThisPieceType];

		//�����ňړ��\�ȃ}�X��Ԃ�����B�����͂��ꂼ�ꌻ�ݑI�𒆂̍��W�ƁA�ړ����̔z��A�z��̒�����n���B
		PieceMoveMapDraw(CurrentSelectFieldRect.RectPositionX, CurrentSelectFieldRect.RectPositionY, CurrentSelectPieceInformation.PieceMapFirstVelue, CurrentSelectPieceInformation.PieceMapLength);
	}
	else if (IsSelectingPiece == false)
	{
		ResetDefaultMap();
	}
}

//�}�X�ڏ�̋�̕]���}�b�v�ɒl�����A���̍��v�����Ƃ߂�֐�
int EnemyPieceMapEvalute(int pieceX, int pieceY, RectTransform *firstMemValue, int length)
{
	//�Ԃ�l�A�ǂꂾ���]�����ꂽ���������ŕԂ�l�Ƃ��ēn��
	int _returnEvalutionValue = 0;

	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		int _distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		int _distanceY = (firstMemValue + _arrayNum)->RectPositionY;	//�ړ����̔z��ɋL�ڂ��ꂽ���W

		int _count = 0;		//����for�����ǂꂾ���񂷂������߂�ϐ�

		//_distanceX��0����Ȃ����_count�ɑ���A0�Ȃ�_distanceY��_count�ɑ������B�ǂ����0�Ȃ�ړ��s�\�Ƃ������ƂŎ��̃��[�v�Ɉڍs����
		if (_distanceX != 0)
		{
			_count = std::abs(_distanceX);
		}
		else if (_distanceY != 0)
		{
			_count = std::abs(_distanceY);
		}
		if (_count == 0)
		{
			continue;
		}
		int _addX = _distanceX / _count;
		int _addY = _distanceY / _count;	//����for���̎���1���[�v���i�ނ��Ƃɂǂꂾ���i�ނ������߂��l

		//_count�̐��������[�v�����A���̂�BoxInfomation��PlayerType�Ŋm�F���s���BPlayerType���v���C���[��NPC�̎��ɂ͎��̃��[�v�Ɉڍs����
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			int _nextY = pieceY + _addY * _moveDis;
			int _nextX = pieceX + _addX * _moveDis;		//���Ɉړ��\���m�F����BoxInformation�z��̔ԍ�

			if (_nextY < RectVerticalNumber &&  _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (BoxInformation[_nextY][_nextX].ThisPlayerType != PlayerType::Enemy)
					{
						_returnEvalutionValue += MAP_VALUE_CANMOVE;		//�ړ��\�ȏꏊ�ł���ꍇ��MAP_VALUE_CANMOVE�����Z�����

						if (BoxInformation[_nextY][_nextX].ThisPlayerType == PlayerType::Player)
						{
							_returnEvalutionValue += MAP_VALUE_CANMOVE;	//�v���C���[����邱�Ƃ��o����ꍇ�͂���Ƀv���X�����
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	return _returnEvalutionValue;	//�Ō�ɕ]�����ꂽ�l��Ԃ�
}

//�]�������l�����ɓ������}�X�ڂ�I������֐�
FieldRectStruct EnemyPieceDetermationByEvalution()
{
	std::vector<FieldRectStruct> _enemyPiecesArray;	//�Ֆʏ�ɑ��݂���NPC�̋�̏��
	std::vector<int> _enemyEvalutionArray;		//��̕]���l�̔z��A��̔z��Ɠ����Ă��鏇�Ԃ͓����ɂ��邱��

	//�s�~���for�����񂷂��Ƃɂ����BoxInformation��S���������āA��̏����擾����
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			if (BoxInformation[_row][_col].ThisPlayerType == PlayerType::Enemy)
			{
				_enemyPiecesArray.push_back(BoxInformation[_row][_col]);
			}
		}
	}

	//��̐�����for�����񂵁A_enemyPieceArray�Ɠ��������̔z������
	for (int _arrayNum=0; _arrayNum < _enemyPiecesArray.size(); _arrayNum++)
	{
		//���݌��Ă���NPC�̋�̈ړ����̔z��̐擪�̒l�Ɣz��̒������擾����
		PieceInformation _currentEnemyPiece = PieceMap[_enemyPiecesArray[_arrayNum].ThisPieceType];

		//EnemyPieceMapEvalute�ɒl��n���A�v�Z�����]���l��z��ɒǉ�����
		_enemyEvalutionArray.push_back(EnemyPieceMapEvalute(_enemyPiecesArray[_arrayNum].RectPositionX, _enemyPiecesArray[_arrayNum].RectPositionY, _currentEnemyPiece.PieceMapFirstVelue, _currentEnemyPiece.PieceMapLength));
	}

	
	int _maxNum = 0;	//�]�����ő�ɂȂ������̔z��̔ԍ�
	int _maxValue = 0;	//���ꂩ���for���Ŏg���ő�l
	
	//�z���S���������āA_maxValue�𒴂�������_maxNum���X�V����
	for (int _arrayNum = 0; _arrayNum < _enemyEvalutionArray.size(); _arrayNum++)
	{
		if (_maxValue < _enemyEvalutionArray[_arrayNum])
		{
			_maxValue = _enemyEvalutionArray[_arrayNum];
			_maxNum = _arrayNum;
		}
	}

	return _enemyPiecesArray[_maxNum];
}

//�G�̈ړ�����v�Z���ADefaultMap�ɉ��Z����֐�
void EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, RectTransform *firstMemValue, int length)
{
	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		int _distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		int _distanceY = (firstMemValue + _arrayNum)->RectPositionY;//�ړ����̔z��ɋL�ڂ��ꂽ���W

		int _count = 0;//����for�����ǂꂾ���񂷂������߂�ϐ�

		//_distanceX��0����Ȃ����_count�ɑ���A0�Ȃ�_distanceY��_count�ɑ������B�ǂ����0�Ȃ�ړ��s�\�Ƃ������ƂŎ��̃��[�v�Ɉڍs����
		if (_distanceX != 0)
		{
			_count = std::abs(_distanceX);
		}
		else if (_distanceY != 0)
		{
			_count = std::abs(_distanceY);
		}
		if (_count == 0)
		{
			continue;
		}
		int _addX = _distanceX / _count;
		int _addY = _distanceY / _count;	//����for���̎���1���[�v���i�ނ��Ƃɂǂꂾ���i�ނ������߂��l

		//_count�̐��������[�v�����A���̂�BoxInfomation��PlayerType�Ŋm�F���s���BPlayerType���v���C���[��NPC�̎��ɂ͎��̃��[�v�Ɉڍs����
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			int _nextY = piecePositionY - _addY * _moveDis;
			int _nextX = piecePositionX - _addX * _moveDis;	//���Ɉړ��\���m�F����BoxInformation�z��̔ԍ�

			if (_nextY < RectVerticalNumber && _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (BoxInformation[_nextY][_nextX].ThisPlayerType != PlayerType::Enemy)
					{
						DefaultMap[_nextY][_nextX] = MAP_VALUE_CANMOVE;	//�ړ��\�ȏꏊ�ł���ꍇ��MAP_VALUE_CANMOVE�����Z�����

						if (BoxInformation[_nextY][_nextX].ThisPlayerType == PlayerType::Player)
						{
							DefaultMap[_nextY][_nextX] = MAP_VALUE_CANMOVE *2;		//�v���C���[����邱�Ƃ��o����ꍇ�͂���Ƀv���X�����
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
}

//DefaultMap����ړ�������肷��֐�
void EnemyMoveDetermation(FieldRectStruct selectRect)
{
	std::vector<RectTransform> rectTransformArray;	//�}�X�ڂ̍��W��z�񉻂������
	std::vector<int> valueArray;	//�}�X�ڂ̕]���̒l��z�񉻂������́A��̔z��Ɠ������ԂŒl�����邱��

	//�s�~���for�����񂷂��Ƃɂ���āADefaultMap��S���������A�l��1�ȏ�̂��̂�T���āA���W�ƒl��z�񉻂���
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			if (DefaultMap[_row][_col] > 0)
			{
				rectTransformArray.push_back({ _col ,_row });	//�]���̒l��1�ȏ�̃}�X�ڂ̍��W
				valueArray.push_back(DefaultMap[_row][_col]);	//�]���̒l
			}

		}
	}

	//�]���̒l�̒��ł̍ő�l
	int _maxValue = *std::max_element(valueArray.begin(), valueArray.end());

	//�ړ���ƂȂ���W�̔z��̔ԍ������̔z��ɓ���
	int _number = 0;

	while (true)
	{
		//�����_���ŕ]���̔z��̔ԍ����w��
		int _arrayNum = GetRand(valueArray.size() -1);
		
		//�����_���Ŏw�肵���z��̗v�f�ƍő�l���ꏏ�Ȃ�_number�ɔz��̔ԍ������āAbreak����
		if (valueArray[_arrayNum] == _maxValue)
		{

			_number = _arrayNum;
			break;
		}
	}

	//�ړ��̊֐��ɒl��n���A�n���l�͌��݂̍��W�A�ړ���̍��W�A�I�𒆂̃}�X�ڂ̏��ł���
	PieceMove(selectRect.RectPositionX, selectRect.RectPositionY, rectTransformArray[_number].RectPositionX, rectTransformArray[_number].RectPositionY, selectRect);
	
}

//�G�̃Q�[���̗�������߂�֐�
void GameProgressForEnemy()
{
	//�͂��߂�DefaultMap������������
	ResetDefaultMap();

	//����ړ�������}�X�ڂ��擾
	FieldRectStruct currentSelectRect = EnemyPieceDetermationByEvalution();

	//�I�񂾃}�X�ڂ̏�ɂ����̈ړ����̔z��̐擪�̒l�Ɣz��̒������擾
	PieceInformation currentSelectPiece = PieceMap[currentSelectRect.ThisPieceType];

	//�ړ����̌v�Z���s���ADefaultMap�ɒl�����Z���Ă���
	EnemyMoveMapCalculation(currentSelectRect.RectPositionX, currentSelectRect.RectPositionY, currentSelectPiece.PieceMapFirstVelue, currentSelectPiece.PieceMapLength);

	//�}�X�ڂ̏���n���A�ړ�������߂�
	EnemyMoveDetermation(currentSelectRect);
	
	PrevPlayer = CurrentGameState;				//PrevPlayer�Ɏ����̏�Ԃ�����
	CurrentGameState = GameState::Confirmation;	//�Ֆʂ̏�Ԃ̊m�F�̃X�e�[�g�ɕύX����
}

//���������s���֐����W�߂��֐�
void GameInitialize()
{
	//�t���X�N���[���ł͂Ȃ��E�B���h�E���[�h�ɕύX
	ChangeWindowMode(TRUE);
	//�E�B���h�E�̏c���Ȃǂ�ݒ�
	SetGraphMode(1920, 1080, 32);
	//DxLib�̏�����
	DxLib_Init();

	//�z����̔z�u��������
	InitializeLoadGraphAndPieceMapSetting();
	InitializeBoxInformaiton();
	PlacementOfPieces();
}

//�Q�[���̃A�b�v�f�[�g�������s���֐�
void GameUpdate()
{
	while (ProcessMessage() == 0)
	{
		//�X�N���[���̈�x�N���A
		ClearDrawScreen();
		//�_�u���o�b�t�@�����O
		SetDrawScreen(DX_SCREEN_BACK);

		//�Q�[���̏�Ԗ��ɏ�����ς���
		switch (CurrentGameState)
		{
		case Title:			//���ݓ��ɏ����Ȃ��A�����Ƀv���C��[�ɃX�e�[�g��n��
			CurrentGameState = GameState::PlayerTurn;
			break;
		case PlayerTurn:	//GameProgressForPlayer�֐�����
			GameProgressForPlayer();
			break;
		case EnemyTurn:		//GameProgressForEnemy�֐�����
			GameProgressForEnemy();
			break;
		case Confirmation:	//�Q�[���̏��s�m�F�̂��߂�ConfirmationWinOrLose�֐�����
			ConfirmationWinOrLose();
			break;
		case Win:			//�����������ɂ�YOUWIN�Ƃ���������\��
			DxLib::DrawFormatString(1000, 900, GetColor(255, 255, 255), "YOU WIN! ");
			break;
		case Lose:			//�s�k�������ɂ�YOULOSE�Ƃ���������\��
			DxLib::DrawFormatString(1000, 900, GetColor(255, 255, 255), "YOU LOSE! ");
			break;
		case NoneState:		//��{�����ɂ͗��Ȃ��͂��Ȃ̂ŃG���[��\��
			DxLib::DrawFormatString(1000, 900, GetColor(255, 255, 255), "ERROR");
			break;
		}
		
		//DrwaMap��Update����
		DrawMap();

		#ifdef  DEBUG
			DebugDefaultMaps();
		#endif //  DEBUG




		//DxLib::DrawFormatString(1000, 950, GetColor(255, 255, 255), "MX:%3d MY:%3d", CurrentSelectFieldRect.RectPositionX, CurrentSelectFieldRect.RectPositionY);

		DxLib::ScreenFlip();
	}
}

//Main�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameInitialize();

	GameUpdate();

	DxLib::DxLib_End();
	return 0;
}