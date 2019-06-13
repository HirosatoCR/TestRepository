#include "EnemyController.h"


controller::EnemyController::EnemyController()
{
}

int controller::EnemyController::EnemyPieceMapEvalute(int pieceX, int pieceY, RectTransform * firstMemValue, int length)
{
	//�Ԃ�l�A�ǂꂾ���]�����ꂽ���������ŕԂ�l�Ƃ��ēn��
	int _returnEvalutionValue = 0;

	int _distanceX = 0;
	int _distanceY = 0;		//�ړ����̔z��ɋL�ڂ��ꂽ���W

	int _count = 0;			//����for�����ǂꂾ���񂷂������߂�ϐ�

	int _addX = 0;
	int _addY = 0;			//����for���̎���1���[�v���i�ނ��Ƃɂǂꂾ���i�ނ������߂��l

	int _nextY = 0;
	int _nextX = 0;			//���Ɉړ��\���m�F����BoxInformation�z��̔ԍ�

	FieldRectStruct _nextBoxInfo;

	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		_distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		_distanceY = (firstMemValue + _arrayNum)->RectPositionY;	

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
			_nextY = pieceY + _addY * _moveDis;
			_nextX = pieceX + _addX * _moveDis;		

			_nextBoxInfo = BoxManager.GetBoxInformation(_nextY, _nextX);

			if (_nextY < RectVerticalNumber &&  _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (_nextBoxInfo.ThisPlayerType != PlayerType::Enemy)
					{
						_returnEvalutionValue += MAP_VALUE_CANMOVE;		//�ړ��\�ȏꏊ�ł���ꍇ��MAP_VALUE_CANMOVE�����Z�����

						if (_nextBoxInfo.ThisPlayerType == PlayerType::Player)
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

FieldRectStruct controller::EnemyController::EnemyPieceDetermationByEvalution()
{
	std::vector<FieldRectStruct> _enemyPiecesArray;	//�Ֆʏ�ɑ��݂���NPC�̋�̏��
	std::vector<int> _enemyEvalutionArray;		//��̕]���l�̔z��A��̔z��Ɠ����Ă��鏇�Ԃ͓����ɂ��邱��

	FieldRectStruct _confirmationBox;
	//�s�~���for�����񂷂��Ƃɂ����BoxInformation��S���������āA��̏����擾����
	for (int _row = 0; _row < RectVerticalNumber; _row++)
	{
		for (int _col = 0; _col < RectHorizontalNumber; _col++)
		{
			_confirmationBox = BoxManager.GetBoxInformation(_row, _col);
			if (_confirmationBox.ThisPlayerType == PlayerType::Enemy)
			{
				_enemyPiecesArray.push_back(_confirmationBox);
			}
		}
	}

	//��̐�����for�����񂵁A_enemyPieceArray�Ɠ��������̔z������
	for (int _arrayNum = 0; _arrayNum < _enemyPiecesArray.size(); _arrayNum++)
	{
		//���݌��Ă���NPC�̋�̈ړ����̔z��̐擪�̒l�Ɣz��̒������擾����
		PieceInformation _currentEnemyPiece = BoxManager.GetPieceMap(_enemyPiecesArray[_arrayNum].ThisPieceType);

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

void controller::EnemyController::EnemyMoveMapCalculation(int piecePositionX, int piecePositionY, RectTransform * firstMemValue, int length)
{
	int _distanceX = 0;
	int _distanceY = 0;
	int _count = 0;//����for�����ǂꂾ���񂷂������߂�ϐ�
	int _addX = 0;
	int _addY = 0;
	int _nextY = 0;
	int _nextX = 0;
	FieldRectStruct _nextBoxInfo;
	for (int _arrayNum = 0; _arrayNum < length; _arrayNum++)
	{
		_distanceX = (firstMemValue + _arrayNum)->RectPositionX;
		_distanceY = (firstMemValue + _arrayNum)->RectPositionY;//�ړ����̔z��ɋL�ڂ��ꂽ���W		

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
		_addY = _distanceY / _count;	//����for���̎���1���[�v���i�ނ��Ƃɂǂꂾ���i�ނ������߂��l

		//_count�̐��������[�v�����A���̂�BoxInfomation��PlayerType�Ŋm�F���s���BPlayerType���v���C���[��NPC�̎��ɂ͎��̃��[�v�Ɉڍs����
		for (int _moveDis = 1; _moveDis <= _count; _moveDis++)
		{
			_nextY = piecePositionY - _addY * _moveDis;
			_nextX = piecePositionX - _addX * _moveDis;	//���Ɉړ��\���m�F����BoxInformation�z��̔ԍ�

			_nextBoxInfo = BoxManager.GetBoxInformation(_nextY, _nextX);

			if (_nextY < RectVerticalNumber && _nextX < RectHorizontalNumber)
			{
				if (_nextY >= 0 && _nextX >= 0)
				{
					if (_nextBoxInfo.ThisPlayerType != PlayerType::Enemy)
					{
						DefaultMap[_nextY][_nextX] = MAP_VALUE_CANMOVE;	//�ړ��\�ȏꏊ�ł���ꍇ��MAP_VALUE_CANMOVE�����Z�����

						if (_nextBoxInfo.ThisPlayerType == PlayerType::Player)
						{
							DefaultMap[_nextY][_nextX] = MAP_VALUE_CANMOVE * 2;		//�v���C���[����邱�Ƃ��o����ꍇ�͂���Ƀv���X�����
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

void controller::EnemyController::EnemyMoveDetermation()
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
		int _arrayNum = GetRand(valueArray.size() - 1);

		//�����_���Ŏw�肵���z��̗v�f�ƍő�l���ꏏ�Ȃ�_number�ɔz��̔ԍ������āAbreak����
		if (valueArray[_arrayNum] == _maxValue)
		{

			_number = _arrayNum;
			break;
		}
	}

	//�ړ��̊֐��ɒl��n���A�n���l�͌��݂̍��W�A�ړ���̍��W�A�I�𒆂̃}�X�ڂ̏��ł���
	BoxManager.PieceMove(CurrentSelectRect.RectPositionX, CurrentSelectRect.RectPositionY, rectTransformArray[_number].RectPositionX, rectTransformArray[_number].RectPositionY, CurrentSelectRect);
}

void controller::EnemyController::ControllerUpdate()
{
	//�͂��߂�DefaultMap������������
	MapManager.ResetDefaultMap();

	//����ړ�������}�X�ڂ��擾
	CurrentSelectRect = EnemyPieceDetermationByEvalution();

	//�I�񂾃}�X�ڂ̏�ɂ����̈ړ����̔z��̐擪�̒l�Ɣz��̒������擾
	CurrentSelectPieceInformation = BoxManager.GetPieceMap(CurrentSelectRect.ThisPieceType);

	//�ړ����̌v�Z���s���ADefaultMap�ɒl�����Z���Ă���
	EnemyMoveMapCalculation(CurrentSelectRect.RectPositionX, CurrentSelectRect.RectPositionY, CurrentSelectPieceInformation.PieceMapFirstVelue, CurrentSelectPieceInformation.PieceMapLength);

	//�}�X�ڂ̏���n���A�ړ�������߂�
	EnemyMoveDetermation();
}