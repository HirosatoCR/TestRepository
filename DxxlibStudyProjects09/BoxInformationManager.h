#pragma once
#include "DxLib.h"
#include "pieceType.h"
#include "playerType.h"
#include <vector>
#include <map>
#include <assert.h>

#define ARRAY_FIRSTMAPVALUE(array) (&array[0])		//�z��̐擪�̒l���擾����}�N��

#define PLAYER_LINE 0		//�v���C���[�̃L���O�Ȃǂ����ԗ�̒l
#define PLAYER_PAWN_LINE 1	//�v���C���[�̃|�[�������ԗ�̒l

#define ENEMY_LINE 7		//NPC�̃L���O�Ȃǂ����ԗ�̒l
#define ENEMY_PAWN_LINE 6	//NPC�̃|�[�������ԗ�̒l

#define KING_POS 2
#define QUEEN_POS 1
#define ROOK_POS 0
#define BISHOP_POS 3

#define WINDOW_SIZE_X 1920
#define WINDOW_SIZE_Y 1080	//��ʂ̉𑜓x
#define COLOR_BIT_NUM 32	//�J���[�r�b�g��

#define COLOR_MAX_VALUE 255	//�F�̃}�b�N�X�̎��̒l

#define MAP_VALUE_CANMOVE 1

//�}�X�̑傫����t�B�[���h�̑傫��
int const BoxSize = 200;
int const FieldWidth = 1600;
int const FieldHeight = 800;

//�c���̃}�X�ڂ̌�
int const RectHorizontalNumber = FieldWidth / BoxSize;
int const RectVerticalNumber = FieldHeight / BoxSize;

//��̏��
struct PieceInfo
{
	PieceType Piece;
	PlayerType Player;
};

//�����̋�z�u��2�����z��Őݒ�
PieceInfo const InitialPiecePlacementMap[RectVerticalNumber][RectHorizontalNumber] = 
{
	{ {PieceType::Rook, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::Rook, PlayerType::Enemy}},
	{ {PieceType::King, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::King, PlayerType::Enemy}},
	{ {PieceType::Queen, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::Queen, PlayerType::Enemy}},
	{ {PieceType::Bishop, PlayerType::Player}, {PieceType::Pawn, PlayerType::Player}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::None, PlayerType::NonPlayer}, {PieceType::Pawn, PlayerType::Enemy}, {PieceType::Bishop, PlayerType::Enemy}}
};

namespace map
{
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

	//�}�X�ڂ̍��W���\���̂ɂ�������
	struct RectTransform
	{
		int RectPositionX;
		int RectPositionY;
	};

	//�e��̏��A�ړ����̐擪�̒l�ƈړ����̔z��̒���
	struct PieceInformation
	{
		RectTransform *PieceMapFirstVelue;	//��q���Ă���ړ����̔z��̐擪�̒l������
		int PieceMapLength;					//�ړ����̔z��̒���������
	};

	//�}�X�ڂ������A���W�Ɖ摜�̃n���h���A���݂̋�̏�ԁA���݂̂ǂ̃v���C���[���ۗL���Ă��邩���Ǘ�����\���́AY��X��RectTransform�Ƌt�Ȃ̂ɒ���
	struct FieldRectStruct
	{
		int RectPositionY = 0;
		int RectPositionX = 0;
		PieceType ThisPieceType;	//��̏��A�|�[�������̍��W�ɂ��鎞�ɂ͂��̒l�̓|�[���ɕς��
		PlayerType	ThisPlayerType;	//���̃}�X�̏�ɍڂ��Ă����̑����͒N����\���l
	};

	//�}�X�ڂ̏����Ǘ�����N���X
	class BoxInformationManager
	{

	public:
		//�R���X�g���N�^�ŏ��������������s
		BoxInformationManager();

		//��̈ړ������A�^�[�Q�b�g�̃}�X�ڂɌ��݂̃}�X�ڂ̏����㏑������֐��A���̊֐���NPC�̃^�[���ł��g�p����
		void PieceMove(int currentPiecePositionX, int currentPiecePositionY, int targetPiecePositionX, int targetPiecePositionY, FieldRectStruct currentPiece);

		//�w�肵���s�Ɨ񂩂�BoxInformation���擾����
		FieldRectStruct GetBoxInformation(int arg_row, int arg_col);

		//��̃^�C�v����ړ������擾����֐�
		PieceInformation GetPieceMap(PieceType arg_pieceType);

		//�f�o�b�O�p�֐�
		void DebugBoxInformation(int X);

		//���݂̋�̌����擾���邽�߂̊֐�
		int GetRemainingPiece(PlayerType arg_playerType);
	private:

		//��̃^�C�v�ƃt�B�[���h�ړ�����R�Â���map
		std::map<PieceType, PieceInformation> PieceMap;

		//PieceMap�̐ݒ�
		void InitializeMoveMap();

		//�t�B�[���h�̌`�ƕR�Â�����Ă���2�����z��̏��A���ꂼ��ǂ̃}�X�ɉ��̎�ނ̋�N�ɑ����Ă��邩�̏�񂪊i�[����Ă���
		FieldRectStruct BoxInformation[RectVerticalNumber][RectHorizontalNumber];

		//BoxInformation�̏�����
		void InitializeBoxInformation();

		//�e��̈ړ����A�ϒ��ϐ��ɕύX
		std::vector<RectTransform> PawnMap = {
		{1,0}
		};

		std::vector<RectTransform> KingMap = {
			{-1,1},{0,1}, {1,1},
			{-1,0},  {1,0},
			{-1,-1}, {0,-1}, {1,-1}
		};

		std::vector<RectTransform> QueenMap = {
			{-10,-10},{0,-10}, {10,-10},
			{-10,0}, {10,0},
			{-10,10}, {0,10}, {10,10},
		};

		std::vector<RectTransform> RookMap = {
			{0,-10},
			{-10,0},  {10,0},
			{0,10}
		};

		std::vector<RectTransform> BishopMap = {
			{-10,-10},{10,-10},
			{-10,10},{10,10},
		};
	};

}