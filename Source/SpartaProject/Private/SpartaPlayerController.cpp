#include "SpartaPlayerController.h"
#include "SpartaGamestate.h"
#include "SpartaGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "kismet/GameplayStatics.h"
#include "Components/TextBlock.h"


ASpartaPlayerController::ASpartaPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)

{
	
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) //�����÷��̾��� ��ü�� �����Ͷ�~
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); //0�� �켱����
			}
	    }
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

//���θ޴� â ����
void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true; //���콺 Ŀ�� ���̰� �ϱ�
			SetInputMode(FInputModeUIOnly()); //UI�� ��� �����ϰ�
		}

		if(UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("StartButtonText")))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart Game")));
			}
			else {
				ButtonText->SetText(FText::FromString(TEXT("Start Game")));
			}
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)));
				}
			}
		}
	}
}
//HUD â ����
void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false; //���콺 Ŀ�� �Ⱥ��̰�
			SetInputMode(FInputModeGameOnly()); //game�� ��� �����ϰ�
		}

		ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
		if (SpartaGameState)
		{
			SpartaGameState->UpdateHUD();
		}
	}
}

void ASpartaPlayerController::StartGame()
{
	if(USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex;
		SpartaGameInstance->TotalScore = 0; //���� ���۽� �� ���� �ʱ�ȭ
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel")); //BasicLevel�� �̵�
	SetPause(false); //���� �Ͻ����� ����
}