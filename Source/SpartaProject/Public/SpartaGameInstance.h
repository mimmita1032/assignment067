#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"

UCLASS()
class SPARTAPROJECT_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USpartaGameInstance();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 TotalScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddScore(int32 Amount);
};
