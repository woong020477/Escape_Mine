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
	UMyAnimInstance(); //������
	

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class UCharacterMovementComponent* Movement;	 //�����Ʈ ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float MoveSpeed;	 //�ӷ��� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Velocity;	//Velocity �ӷ��� �����ϴ� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool isFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float ZSpeed;	 //�������� �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool Iscrouch;	 //�ɾ��ִ��� Ȯ��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool IsRunning;	 //�ٴ��� Ȯ��
};
