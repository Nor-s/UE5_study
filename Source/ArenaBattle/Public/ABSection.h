// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABSection.generated.h"

UCLASS()
class ARENABATTLE_API AABSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 출입구
	UPROPERTY(VisibleAnywhere, Category=Mesh, Meta = (AllowPrivateAccess=true))
	TArray<UStaticMeshComponent*> GateMeshes;

	UPROPERTY(VisibleAnywhere, Category=Trigger, Meta = (AllowPrivateAccess = true))
	TArray<UBoxComponent*> GateTriggers;
	
	// 섹션의 배경
	UPROPERTY(VisibleAnywhere, Category=Mesh, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;

	// 섹션의 중앙
	UPROPERTY(VisibleAnywhere, Category=Trigger, Meta=(AllowPrivateAccess=true))
	UBoxComponent* Trigger;
		
};
