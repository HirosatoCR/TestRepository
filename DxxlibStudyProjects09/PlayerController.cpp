#include "PlayerController.h"

//�ړ��ł���}�X�ڂ�h��Ԃ������A���݂̒n�_����ړ����̒l�����Z�����n�_�܂ł����ꂼ��̍��W��1�����Z���āA�ړ��\���ǂ����m���߂Ă����������s���Ă���
void controller::PlayerController::PieceMoveMapDraw(int piecePositionX, int piecePositionY, map::RectTransform * firstMemValue, int length)
{
	int _addNum = 0;	//�ړ��o����ӏ��̐��A0�̎��ɂ̓L�����I����Ԃ���������
	int _distanceX = 0;
	int _distanceY = 0;
	int _count = 0;		//for�����ǂꂾ���񂷂������߂�ϐ�
	int _addX = 0;
	int _addY = 0;		//����for���̎���1���[�v���i�ނ��Ƃɂǂꂾ���i�ނ������߂��l
	int _upperY = 0;
	int _upperX = 0;	//���Ɉړ��\���m�F����BoxInformation�z��̔ԍ�

	map::FieldRectStruct nextBoxInfo;	//���݊m�F���̃}�X�̏��

	for (int _arrayNumber = 0; _arrayNumber < length; _arrayNumber++)
	{
		_distanceX = (firstMemValue + _arrayNumber)->RectPositionX;
		_distanceY = (firstMemValue + _arrayNumber)->RectPositionY;	//�ړ����̔z��ɋL�ڂ��ꂽ���W

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

		_addX = _distanceX / _count;
		_addY = _distanceY / _count;

		//_count�̐��������[�v�����A���̂�BoxInfomation��PlayerType�Ŋm�F���s���BPlayerType���v���C���[��NPC�̎��ɂ͎��̃��[�v�Ɉڍs����
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			_upperY = piecePositionY + _addY * _moveDis;
			_upperX = piecePositionX + _addX * _moveDis;

			if (_upperY < RectVerticalNumber && _upperX < RectHorizontalNumber && _upperY >= 0 && _upperX >= 0)
			{
				nextBoxInfo = BoxManager->GetBoxInformation(_upperY, _upperX);

				if (nextBoxInfo.ThisPlayerType != PlayerType::Player)
				{
					MapManager.SetDefaultMap(_upperY, _upperX, MAP_VALUE_CANMOVE);
					_addNum++;

					if (nextBoxInfo.ThisPlayerType == PlayerType::Enemy)
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
}

//�}�E�X�̃N���b�N�����������̂�True��Ԃ��֐�
bool controller::PlayerController::PieceSelectPressed()
{
	//���͂������ǂ������m�F����ϐ�
	bool _returnBool = false;

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
}

//std::clamp�̂悤��3�̒l��n���Ƃ͂��߂̒l�����2�̒l�̊Ԃɂ��邩�𔻒肵�A�ԂɂȂ��ꍇ�͍ő�l���ŏ��l�ɒu��������֐�
int controller::PlayerController::InsteadOfClamp(int currentValue, int minValue, int maxValue)
{
	//�Ԃ�l�A��{�I�ɂ͈�����Ԃ����A�ŏ���菬�����ꍇ��ő���傫���ꍇ�͍ŏ��ƍő傪���ꂼ��Ԃ�l�Ƃ��ē���
	int _returnValue = currentValue;

	if (currentValue >= maxValue)
	{
		_returnValue = maxValue - 1;
	}
	else if (currentValue < minValue)
	{
		_returnValue = minValue;
	}

	return _returnValue;
}

//�}�E�X���N���b�N�����ӏ���FieldRectStruct���擾����֐�
void controller::PlayerController::MouseClickGetFieldRectStruct()
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
		map::FieldRectStruct _clickSelectBoxInfromation = BoxManager->GetBoxInformation(_ClickRectY, _ClickRectX);

		int _mapValue = 0;

		//����if���̓L������I�����Ă��Ȃ���ԂŃN���b�N�����Ƃ��ɗ����
		if (IsSelectingPiece == false)
		{
			//�I�������}�X��PlayerType��Player�Ȃ�L�����I�𒆂ɂȂ�A�I�𒆂�BoxInfomation������CurrentSelectFieldRect�ɑI�������}�X�ڂ̏�񂪐ݒ肳���
			if (_clickSelectBoxInfromation.ThisPlayerType == PlayerType::Player)
			{
				IsSelectingPiece = !IsSelectingPiece;
				CurrentSelectRect = _clickSelectBoxInfromation;
			}
		}
		//�L������I��������ԂŃ}�E�X���N���b�N�������ɏ����������
		else if (IsSelectingPiece == true)
		{
			//�͂��߂ɃL�����I����Ԃ���������
			IsSelectingPiece = !IsSelectingPiece;

			//�I�������}�X��PlayerType��Player����Ȃ��Ȃ玟�̏����������
			if (_clickSelectBoxInfromation.ThisPlayerType != PlayerType::Player)
			{
				_mapValue = MapManager.GetDefaultMap(_ClickRectY, _ClickRectX);
				//�Ȃ����I�������}�X���ړ��o����ꏊ�ł���Ȃ�ړ��̏����ɂȂ����
				if (_mapValue == MAP_VALUE_CANMOVE)
				{
					int _lastNumX = CurrentSelectRect.RectPositionX;
					int _lastNumY = CurrentSelectRect.RectPositionY;	//���݂̑I�����Ă���}�X�̍��W

					_clickSelectBoxInfromation = BoxManager->GetBoxInformation(_lastNumY, _lastNumX);
					BoxManager->PieceMove(_lastNumX, _lastNumY, _ClickRectX, _ClickRectY, _clickSelectBoxInfromation);	//�ړ�����

					TurnChange = false;
				}
			}
		}
	}
}

//�������z�֐����I�[�o�[���C�h�����֐��A�Q�[���̃��[�v�����ōs���Ăق��������������ɂ��ׂċL�q����
void controller::PlayerController::ControllerUpdate()
{
	//
	TurnChange = true;
	MouseClickGetFieldRectStruct();

	//�L�������I�����ꂽ�ꍇ�͂��̊֐��������Ă΂��悤�ɂȂ�B
	if (IsSelectingPiece == true)
	{
		//�����Ō��ݑI�𒆂̃}�X�̏�񂪎����Ă����̎�ނƕR�Â��Ă���A�ړ����̔z��̐擪�̒l�Ɣz��̒������擾����
		CurrentSelectPieceInformation = BoxManager->GetPieceMap(CurrentSelectRect.ThisPieceType);

		//�����ňړ��\�ȃ}�X��Ԃ�����B�����͂��ꂼ�ꌻ�ݑI�𒆂̍��W�ƁA�ړ����̔z��A�z��̒�����n���B
		PieceMoveMapDraw(CurrentSelectRect.RectPositionX, CurrentSelectRect.RectPositionY, CurrentSelectPieceInformation.PieceMapFirstVelue, CurrentSelectPieceInformation.PieceMapLength);
	}
	else if (IsSelectingPiece == false)
	{
		MapManager.ResetDefaultMap();
	}
	MapManager.DrawMap(BoxManager);
}

//�����������A��Ƀ������m�ۂ�O���t�B�b�N�n���h���̏��������s���֐�
void controller::PlayerController::Initialize(map::BoxInformationManager * arg_box)
{
	BoxManager = new map::BoxInformationManager;
	BoxManager = arg_box;		//�������m�ہA�����Ƃ��Ă���BoxInformationManager�����l���d�v�Ȃ̂ŎQ�Ƃ��󂯎���ĕϐ��ɃR�s�[

	MapManager.InitializeGraphMap();
}

