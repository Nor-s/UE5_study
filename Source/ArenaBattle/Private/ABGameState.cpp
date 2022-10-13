#include "ABGameState.h"

AABGameState::AABGameState()
{
	TotalGameScore = 0;
}

int32 AABGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AABGameState::AddGameScore()
{
	TotalGameScore++;
}