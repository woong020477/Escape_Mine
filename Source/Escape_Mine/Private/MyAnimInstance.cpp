// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework//CharacterMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{

}

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		MoveSpeed = Velocity.Size2D();
		isFalling = Movement->IsFalling();
		ZSpeed = Movement->Velocity.Z;
		Iscrouch = Movement->IsCrouching();
		IsRunning=
	}
}