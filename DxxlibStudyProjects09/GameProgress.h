#pragma once
#include "PlayerController.h"
#include "EnemyController.h"
#include "GameState.h"


namespace progress
{
	//ゲームの実際の処理をまとめているクラス
	class GameProgress
	{
	public:
		//ゲームの初期化処理、main関数が動き始めた時はじめに流れる処理をまとめている
		void GameInitialize();

		//ゲームのループ部分の関数
		void GameUpdate();

		//ゲームのループ部分の中でのインゲームの部分のループを扱う関数
		void InGameUpdate();

		//ゲーム終了時の処理を扱う関数
		void GameEnd();

		//駒の数から勝敗の確認を行う関数
		void ConfirmationWinOrLose();

		//ゲームのステートを変更する関数
		void SetCurrentGameState(GameState arg_nextState) { CurrentGameState = arg_nextState; }

		//現在のプレイヤーのステートを変更する関数
		void SetCurrentPlayerType(PlayerType arg_playerType) { CurrentPlayerType = arg_playerType; }
	private:
		//各コントローラー
		controller::PlayerController PlayerController;
		controller::EnemyController EnemyController;

		//ゲームの進行ステート
		GameState CurrentGameState = GameState::Title;

		//現在のプレイ中のプレイヤーのステート
		PlayerType CurrentPlayerType;

		//現在のBoxInformationの実体
		map::BoxInformationManager CurrentUseBoxInformation;
	};
}

