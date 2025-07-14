#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)	
struct FItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName; // ������ ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> ItemClass; // ������ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance; // ������ ���� Ȯ��
	
};
