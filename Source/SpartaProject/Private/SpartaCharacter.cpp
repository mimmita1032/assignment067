#include "SpartaCharacter.h"
#include "SpartaplayerController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include <SpartaGameState.h>

// ������: ������Ʈ �ʱ�ȭ �� �⺻�� ����
ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false; // Tick �Լ� ��Ȱ��ȭ (�ʿ��� ��� true�� ����)

	// SpringArm ������Ʈ ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true; // �÷��̾��� ���콺 ȸ���� ���� ȸ��

	// ī�޶� ������Ʈ ����
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // ī�޶�� ȸ������ ���� (SpringArm�� ȸ��)

	// ���� ������Ʈ (ü�� ǥ�ÿ�) ����
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh()); // �޽ÿ� ����
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen); // ȭ�� ���� ��ġ

	// �̵� �ӵ� ���� ���� �ʱ�ȭ
	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// ü�� �ʱ�ȭ
	Maxhealth = 100.0f;
	Health = Maxhealth;
}

// ���� ���� �� ȣ��Ǵ� �Լ�
void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP(); // ���� �� ���� ü�� �ؽ�Ʈ ������Ʈ
}

// �Է� ���ε� ����
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ��ȭ �Է� ������Ʈ ĳ����
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			// �̵� �Է� ���ε�
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Move);
			}
			// ���� �Է� ���ε�
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StopJump); // ���ÿ� ���ε��ǹǷ� ���� ������ ���� ���ǹ����� �и�
			}
			// ���콺 �þ� ȸ�� ���ε�
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Look);
			}
			// �޸��� �Է� ���ε�
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopSprint);
			}
		}
	}
}

// �̵� ó��
void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	// ����/���� �̵�
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	// ��/�� �̵�
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

// ���� ����
void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

// ���� ����
void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

// ���콺 �þ� ȸ��
void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X); // �¿� ȸ��
	AddControllerPitchInput(LookInput.Y); // ���� ȸ��
}

// �޸��� ����
void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// �޸��� ����
void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

// ���� ü�� ��ȯ (UI���� ȣ���� �� ����)
float ASpartaCharacter::GetHealth() const
{
	return Health;
}

// ü�� ȸ�� ó��
void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, Maxhealth);
	UpdateOverheadHP();
}

// ������ ó�� (���� �޾��� ��)
float ASpartaCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// ü�� ���� �� ����
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, Maxhealth);
	UpdateOverheadHP();

	// ��� ó��
	if (Health <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

// ��� ó�� �Լ�
void ASpartaCharacter::OnDeath()
{
	// GameState�� �����ͼ� ���� ���� ó��
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

// ü�� ������ ǥ�õǴ� HP �ؽ�Ʈ ����
void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	// "OverHeadHP" ��� �̸��� ���� �ؽ�Ʈ ��� ã��
	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		// �ؽ�Ʈ ����
		HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.0f / %.0f"), Health, Maxhealth)));
	}
}
