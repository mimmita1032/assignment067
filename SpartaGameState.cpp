#include "SpartaGameState.h"  
#include "Kismet/GameplayStatics.h"  
#include "GameFramework/PlayerController.h"
#include "SpawnVolume.h"
#include "SpartaGameInstance.h"
#include "CoinItem.h"  
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include <SpartaPlayerController.h>

ASpartaGameState::ASpartaGameState()  
{  
	Score = 0;  
	SpawnedCoinCount = 0;  
	CollectedCoinCount = 0;  
	LevelDuration = 30.0f; 
	CurrentLevelIndex = 0; 
	MaxLevels = 3; 
	MaxWaveCount = 3;//최대 웨이브3으로
	 
	
}  

void ASpartaGameState::BeginPlay()  
{  
	Super::BeginPlay();  
	
	StartLevel();  

	GetWorldTimerManager().SetTimer(  
		HUDUpdateTimerHandle,  
		this,  
		&ASpartaGameState::UpdateHUD,  
		0.1f,  
		true  //반복
	);
}  

int32 ASpartaGameState::GetScore() const  
{  
	return Score;  
}  

void ASpartaGameState::SetScore(int32 Amount)  
{  
	if (UGameInstance* GameInstance = GetGameInstance())  
	{  
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);  
		if (SpartaGameInstance)  
		{  
			SpartaGameInstance->AddScore(Amount);  
		}  
	} 
}  

void ASpartaGameState::StartLevel()  
{
	CurrentWaveIndex = 0; //레벨 시작시 초기화
	// HUD
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD(); 
		}
	}
//레벨 인덱스?
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}
//wave
	//웨이브 시작
	StartWave();
//타이머 쓰던거
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeup,
		LevelDuration,
		false
		);
}
//wave
void ASpartaGameState::StartWave()
{

	//현재 웨이브 출력
	UE_LOG(LogTemp, Warning, TEXT("Wave %d"), CurrentWaveIndex + 1);

	//이번 웨이브 코인 수 
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	//스폰 가능한 액터 모두
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	//웨이브당 코인 수 
	const int32 CoinsPerWave = 5;

	
	if (FoundVolumes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("spawnvolume 없음"));
		EndWave();
		return;
	}
	//코인 수만큼 스폰
	for (int32 i = 0; i < CoinsPerWave; ++i)
	{
		//스폰 볼륨이 있을 때
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				//랜덤 아이템 스폰
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				//코인 아이템 스폰시 카운트 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	UpdateHUD();	
}
//wave
void ASpartaGameState::EndWave()
{
	//이번 웨이브 끝 출력
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Finish"), CurrentWaveIndex + 1);
	//다음 웨이브로
	CurrentWaveIndex++;

	if (CurrentWaveIndex < MaxWaveCount)
	{
		StartWave();
	}
	else
	{
		EndLevel(); //웨이브 끝나면 레벨종료
	}
}
void ASpartaGameState::OnLevelTimeup()  
{  
	EndLevel();  
}  

void ASpartaGameState::OnCoinCollected()  
{  
	CollectedCoinCount++;  

	UE_LOG(LogTemp, Warning, TEXT("Coin collected! Total collected: %d / %d"),  
		CollectedCoinCount,  
		SpawnedCoinCount);  
	//모든 코인 수집 후 EndWave
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)  
	{  
		EndWave();  //Level에서 Wave로 변경
	}  
}  

void ASpartaGameState::EndLevel()  
{  
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);  
	CurrentLevelIndex++;  

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SetScore(Score);
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)  
	{  
		OnGameOver();  
		return;  
	}  
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))  
	{  
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}  
	else
	{
		OnGameOver();
	}
}  

void ASpartaGameState::OnGameOver()  
{  
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true); // 게임 일시 정지
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()  
{  
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())  
	{  
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))  
		{  
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())  
			{  
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))  
				{  
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time Remaining: %.2f"), RemainingTime)));  
				}  
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))  
				{  
					if (UGameInstance* GameInstance = GetGameInstance())  
					{  
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);  
						if (SpartaGameInstance)  
						{  
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}  
					}  
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))  
				{  
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d/%d"), CurrentLevelIndex + 1, MaxLevels)));  
				}
				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("WaveText"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d / %d"), CurrentWaveIndex + 1, MaxWaveCount)));
				}
			}  
		}  
	}  
}