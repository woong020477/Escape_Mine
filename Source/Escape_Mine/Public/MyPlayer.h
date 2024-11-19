// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPlayer.generated.h"

UCLASS()
class ESCAPE_MINE_API AMyPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AMyPlayer();
    void PlayerMove();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //ī�޶� ������ ó��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;

    //ī�޶� ó��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CamComp;

    //��ǲ-
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* InputAction_Turn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* InputAction_LookUp;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InputAction_Move;
    UPROPERTY(EditAnywhere, Category = PlayerSetting)
    float walkSpeed=400;
    FVector direction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InputAction_Jump;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InputAction_Crouch;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprint")
    bool IsSprinting;

    // (�ٱ�) �ִ� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
    float SprintMaxSpeed = 600.f;           //�ӵ� ���� ���⼭ �ϸ� ��.

    // (�⺻) �̵� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
    float DefaultMaxSpeed = 450.f;          //�ӵ� ���� ���⼭ �ϸ� ��.

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InputAction_Sprint;


    void Move(const struct FInputActionValue& inputValue);
    void Turn(const FInputActionValue& InputValue);
    void LookUp(const FInputActionValue& InputValue);
    void Input_Jump(const struct FInputActionValue& inputValue);
    void StartCrouch(const struct FInputActionValue& inputValue);
    void StopCrouch(const struct FInputActionValue& inputValue);
    void StartSprint(const struct FInputActionValue& inputValue);
    void StopSprint(const struct FInputActionValue& inputValue);
};
