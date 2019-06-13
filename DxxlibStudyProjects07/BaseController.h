#include "DrawMapManager.h"

namespace controller
{
	
	class BaseController
	{
	protected:
		PlayerType CurrentPlayerType;

		map::DrawMapManager MapManager;
		map::BoxInformationManager BoxManager;
		virtual void ControllerUpdate() =0;

		FieldRectStruct CurrentSelectRect;

		PieceInformation CurrentSelectPieceInformation;

	public:
		BaseController();

		void SetPlayerType(PlayerType arg_playerState);
		PlayerType GetPlayerType() { return CurrentPlayerType; }
	};
}

