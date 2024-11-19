#include "MyPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h" 


AMyPlayer::AMyPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    IsSprinting = false;

    // ���̷��� �޽� ��������
    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Survival_Character/Meshes/SK_Survival_Character.SK_Survival_Character'"));
    if (TempMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(TempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
    }

    // ������ �� ����
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->SetRelativeLocation(FVector(0, 70, 90));
    SpringArmComp->TargetArmLength = 400;
    SpringArmComp->bUsePawnControlRotation = true;

    // ī�޶� ������Ʈ ����
    CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
    CamComp->SetupAttachment(SpringArmComp);
    CamComp->bUsePawnControlRotation = false;

    // �÷��̾� ȸ��
    bUseControllerRotationYaw = true;
}

void AMyPlayer::BeginPlay()
{
    Super::BeginPlay();

    
    // Input Mapping Context �߰�
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
    SetActorLocation(P);                                ���� ��ӿ ����*/

    //�÷��̾� �̵� ó��
    direction = FTransform(GetControlRotation()).TransformVector(direction);
    AddMovementInput(direction);
    direction = FVector::ZeroVector;
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
    }
}

void AMyPlayer::Turn(const FInputActionValue& InputValue) // ī�޶� �� (Yaw)
{
    float Value = InputValue.Get<float>();
    AddControllerYawInput(Value);
}

void AMyPlayer::LookUp(const FInputActionValue& InputValue) //ī�޶� �� (Pitch)
{
    float Value = InputValue.Get<float>();
    AddControllerPitchInput(Value); 
}

void AMyPlayer::Move(const struct FInputActionValue& inputValue)  //������
{
    FVector2D value = inputValue.Get<FVector2D>();
    direction.X = value.X;
    direction.Y = value.Y;
}
void AMyPlayer::Input_Jump(const struct FInputActionValue& inputValue) 
{
    Jump();
}
void AMyPlayer::StartCrouch(const FInputActionValue& inputValue) //�ɱ�
{
    if (!bIsCrouched) 
    {
        Crouch(); 
    }
}

void AMyPlayer::StopCrouch(const FInputActionValue& inputValue) //�ɾƼ� �ȱ�
{
    if (bIsCrouched) 
    {
        UnCrouch(); 
    }
}

void AMyPlayer::StartSprint(const FInputActionValue& inputValue) //�ٱ� ����
{
    IsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = SprintMaxSpeed;
}
void AMyPlayer::StopSprint(const FInputActionValue& inputValue) //�ȱ�
{
    IsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
}