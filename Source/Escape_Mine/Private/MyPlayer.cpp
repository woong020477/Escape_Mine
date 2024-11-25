#include "MyPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"



AMyPlayer::AMyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    IsSprinting = false;
    IsCrouch = false;
    IsAttacking = false;
    IsPickaxeVisble = false;
    comboCnt = 0;
    HP = 100;

    // 스켈레톤 메시 가져오기
    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
    if (TempMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(TempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
    }

    // 스프링 암 장착
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->SetRelativeLocation(FVector(0, 0, 90));
    SpringArmComp->TargetArmLength = 400;
    SpringArmComp->bUsePawnControlRotation = true;
    /*카메라 렉 설정 (앉기 시 카메라가 자연스럽게 내려옴 필요 시 주석처리 o)*/
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->bEnableCameraRotationLag = true;
    SpringArmComp->CameraLagSpeed = CameraLagSpeed;

    // 카메라 컴포넌트 장착
    CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
    CamComp->SetupAttachment(SpringArmComp);
    CamComp->bUsePawnControlRotation = false;


    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    CapsuleComp->SetupAttachment(GetMesh(), TEXT("Hand_r"));
    CapsuleComp->SetRelativeLocation(FVector(-27.81f, 1.7f, 24.5f));

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(GetMesh(), TEXT("Hand_r"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Scene_OldMine/Assets/Megascans/3D_Assets/His_Med_Tools_Pickaxe_Metal_Old_03"));
    StaticMesh->SetStaticMesh(MeshAsset.Object);
    StaticMesh->SetRelativeLocation(FVector(-44.24f, 3.54f, 31.8f));
    StaticMesh->SetRelativeRotation(FRotator(0, 50, 0));

    StaticMesh->SetVisibility(IsPickaxeVisble);

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimiInstance(TEXT("/Game/Animations/ABP_MyPlayerAni.ABP_MyPlayerAni_C"));

    if (AnimiInstance.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimiInstance.Class);                                       //해당 캐릭터의 애니메이션 클래스가 없을 시 자동 적용
    }
}

void AMyPlayer::BeginPlay()
{
    Super::BeginPlay();


    // Input Mapping Context 추가
    auto PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController)
    {
        auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (Subsystem)
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }

    
}

void AMyPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    PlayerMove();
}
void AMyPlayer::PlayerMove()
{
    /*FVector P0 = GetActorLocation();
    FVector vt = direction * walkSpeed * DeltaTime;
    FVector P = P0 + vt;
    SetActorLocation(P);                                기초 등속운동 구현*/

    //플레이어 이동 처리
    if (!IsAttacking) 
    {
        direction = FTransform(GetControlRotation()).TransformVector(direction);
        AddMovementInput(direction);
        direction = FVector::ZeroVector;
    }
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    if (PlayerInput)
    {
        PlayerInput->BindAction(InputAction_Turn, ETriggerEvent::Triggered, this, &AMyPlayer::Turn);
        PlayerInput->BindAction(InputAction_LookUp, ETriggerEvent::Triggered, this, &AMyPlayer::LookUp);
        PlayerInput->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
        PlayerInput->BindAction(InputAction_Jump, ETriggerEvent::Triggered, this, &AMyPlayer::Input_Jump);
        PlayerInput->BindAction(InputAction_Crouch, ETriggerEvent::Triggered, this, &AMyPlayer::StartCrouch);
        PlayerInput->BindAction(InputAction_Crouch, ETriggerEvent::Completed, this, &AMyPlayer::StopCrouch);
        PlayerInput->BindAction(InputAction_Sprint, ETriggerEvent::Triggered, this, &AMyPlayer::StartSprint);
        PlayerInput->BindAction(InputAction_Sprint, ETriggerEvent::Completed, this, &AMyPlayer::StopSprint);
        PlayerInput->BindAction(InputAction_Attack, ETriggerEvent::Started, this, &AMyPlayer::Attack);
        PlayerInput->BindAction(InputAction_Ouch, ETriggerEvent::Triggered, this, &AMyPlayer::Ouch);
    }
}

void AMyPlayer::Turn(const FInputActionValue& InputValue) // 카메라 턴 (Yaw)
{
    if (!IsAttacking) {
        float Value = InputValue.Get<float>();
        AddControllerYawInput(Value);
    }
}

void AMyPlayer::LookUp(const FInputActionValue& InputValue) //카메라 턴 (Pitch)
{
    float Value = InputValue.Get<float>();
    AddControllerPitchInput(Value);
}

void AMyPlayer::Move(const struct FInputActionValue& inputValue)  //움직임
{
    FVector2D value = inputValue.Get<FVector2D>();
    direction.X = value.X;
    direction.Y = value.Y;
}
void AMyPlayer::Input_Jump(const struct FInputActionValue& inputValue)
{
    if (!IsAttacking) 
    {
        Jump();
    }
}
void AMyPlayer::StartCrouch(const FInputActionValue& inputValue) //앉기
{
    if (!IsAttacking) 
    {
        if (!bIsCrouched)
        {
            IsCrouch = true;
            Crouch();
        }
    }
}

void AMyPlayer::StopCrouch(const FInputActionValue& inputValue) //앉아서 걷기
{
    if (!IsAttacking) 
    {
        if (bIsCrouched)
        {
            IsCrouch = false;
            UnCrouch();
        }
    }
}

void AMyPlayer::StartSprint(const FInputActionValue& inputValue) //뛰기 시작
{
    if (!IsAttacking) 
    {
        IsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintMaxSpeed;
    }
}
void AMyPlayer::StopSprint(const FInputActionValue& inputValue) //걷기
{
    if (!IsAttacking) 
    {
        IsSprinting = false;
        GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
    }
}
void AMyPlayer::Attack(const FInputActionValue& inputValue)
{
    if (IsAttacking&& IsPickaxeVisble)
    {
        comboCnt++;
        IsAttacking = false;
        if (comboCnt == 2)
        {
            comboCnt = 0;
        }
    }
}
void AMyPlayer::Ouch(const FInputActionValue& inputValue) 
{
    HP -= 10;
}