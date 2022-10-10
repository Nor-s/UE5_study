// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*
IWYU- 엔진 클래스의 선언을 모아둔 헤더 사용
// #include "EngineMinimal.h"
*/
#include "ArenaBattle.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/* UE5은 언리얼 런타임을 통해 객체가 더 이상 사용되지 않으면 할당된 메모리를 자동으로 소멸시키는 기능 제공
	 * UPROPERTY 매크로 사용해야함
	 * 이는 언리얼 오브젝트 객체에만 사용 가능
	 * 언리얼 오브젝트 == 언리얼 런타임에 의해 관리되는 C++ 객체 == 콘텐츠를 구성하는 객체
	 */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Body;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Water;
	
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent *Light;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *Splash;
	
	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;
	
	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;
private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed;	
};

/**
 * 언리얼 엔진 언리얼 오브젝트 클래스 규칙
 * - 클래스 선언 매크로 UCLASS(), GENERATED_BODY()
 * - 클래스 이름 접두사  A: 액터 클래스, U: 액터가 아닌 클래스
 * - generated.h 헤더파일: 언리얼 헤더 툴에 의해 자동으로 생성되는 부가 파일 (컴파일 과정에서 발생)
 * - 외부 모듈에의 공개 여부: DLL 내 클래스 정보를 외부에 공개하는 _delclspec(dllexport) 키워드 => '모듈명_API' 키워드
 *		- 이 키워드가 없으면 다른 모듈에서 해당 객체 접근 불가
 *
 */
