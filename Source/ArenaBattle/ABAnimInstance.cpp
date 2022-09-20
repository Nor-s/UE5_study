// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"


UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
}


void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	
	if(const APawn* const Pawn = TryGetPawnOwner(); ::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if(Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

