#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)	
struct FItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName; // 아이템 스폰 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> ItemClass; // 아이템 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance; // 아이템 스폰 확률
	
};
