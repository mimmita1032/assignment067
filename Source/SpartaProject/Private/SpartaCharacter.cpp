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

// 생성자: 컴포넌트 초기화 및 기본값 설정
ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false; // Tick 함수 비활성화 (필요할 경우 true로 변경)

	// SpringArm 컴포넌트 설정
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true; // 플레이어의 마우스 회전에 따라 회전

	// 카메라 컴포넌트 설정
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // 카메라는 회전하지 않음 (SpringArm만 회전)

	// 위젯 컴포넌트 (체력 표시용) 설정
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh()); // 메시에 부착
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen); // 화면 기준 위치

	// 이동 속도 관련 변수 초기화
	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// 체력 초기화
	Maxhealth = 100.0f;
	Health = Maxhealth;
}

// 게임 시작 시 호출되는 함수
void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP(); // 시작 시 위젯 체력 텍스트 업데이트
}

// 입력 바인딩 설정
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 강화 입력 컴포넌트 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			// 이동 입력 바인딩
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Move);
			}
			// 점프 입력 바인딩
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StopJump); // 동시에 바인딩되므로 실제 동작은 내부 조건문으로 분리
			}
			// 마우스 시야 회전 바인딩
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Look);
			}
			// 달리기 입력 바인딩
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopSprint);
			}
		}
	}
}

// 이동 처리
void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	// 전진/후진 이동
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	// 좌/우 이동
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

// 점프 시작
void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

// 점프 중지
void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

// 마우스 시야 회전
void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X); // 좌우 회전
	AddControllerPitchInput(LookInput.Y); // 상하 회전
}

// 달리기 시작
void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// 달리기 종료
void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

// 현재 체력 반환 (UI에서 호출할 수 있음)
float ASpartaCharacter::GetHealth() const
{
	return Health;
}

// 체력 회복 처리
void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, Maxhealth);
	UpdateOverheadHP();
}

// 데미지 처리 (공격 받았을 때)
float ASpartaCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 체력 감소 및 제한
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, Maxhealth);
	UpdateOverheadHP();

	// 사망 처리
	if (Health <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

// 사망 처리 함수
void ASpartaCharacter::OnDeath()
{
	// GameState를 가져와서 게임 오버 처리
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

// 체력 위젯에 표시되는 HP 텍스트 갱신
void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	// "OverHeadHP" 라는 이름을 가진 텍스트 블록 찾기
	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		// 텍스트 갱신
		HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.0f / %.0f"), Health, Maxhealth)));
	}
}
