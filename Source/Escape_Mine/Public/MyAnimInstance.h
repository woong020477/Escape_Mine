// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_MINE_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance(); //생성자
	

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class UCharacterMovementComponent* Movement;	 //무브먼트 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float MoveSpeed;	 //속력을 저장

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Velocity;	//Velocity 속력을 저장하는 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool isFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float ZSpeed;	 //떨어지는 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool Iscrouch;	 //앉아있는지 확인

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool IsRunning;	 //뛰는지 확인
};
