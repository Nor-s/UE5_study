


#include "ABPlayerState.h"

AABPlayerState::AABPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
}

int32 AABPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void AABPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	CharacterLevel = 5;
	GameScore = 0;
}