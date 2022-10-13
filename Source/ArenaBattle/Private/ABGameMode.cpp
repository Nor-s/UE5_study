// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ABGameState.h"


// 주의: 폰을 생성하고 지정하는 것이 아니라, 클래스 정보를 지정함
// 멀티를 고려한다면, 미리 폰을 만들어두는 것이 아니라 클래스 정보만 저장, 플레이어 입장 시 생성하는 것이 합리적
AABGameMode::AABGameMode()
{
	// 언리얼 오브젝트의 클래스 정보는 언리얼 헤더 툴에 의해 자동으로 생성, StaticClass() 함수 호출해 가져옴
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();
	GameStateClass = AABGameState::StaticClass();
}
void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABGameState = Cast<AABGameState>(GameState);
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("Super::PostLogin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin"));

	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();
}
void AABGameMode::AddScore(class AABPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
		if ((nullptr != ABPlayerController) && (ScoredPlayer == ABPlayerController))
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}
	ABGameState->AddGameScore();
}