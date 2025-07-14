#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARandomPlatformSpawner.generated.h"

// 이 클래스는 일정 시간마다 랜덤한 위치에 발판을 생성하는 스포너 역할을 한다.
UCLASS()
class SPARTAPROJECT_API ARandomPlatformSpawner : public AActor
{
	GENERATED_BODY()

public:
	// 생성자
	ARandomPlatformSpawner();

protected:
	// 게임 시작 시 호출되는 함수
	virtual void BeginPlay() override;

private:
	// 발판 생성 주기 (초 단위, 예: 3초마다 생성)
	UPROPERTY(EditAnywhere)
	float SpawnInterval;

	// 생성될 X, Y 위치의 최소 좌표
	UPROPERTY(EditAnywhere)
	FVector2D SpawnMin;

	// 생성될 X, Y 위치의 최대 좌표
	UPROPERTY(EditAnywhere)
	FVector2D SpawnMax;

	// 생성할 회전 발판 클래스 (블루프린트에서 지정)
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RotatingPlatformClass;

	// 생성할 이동 발판 클래스 (블루프린트에서 지정)
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MovingPlatformClass;

	// 반복 타이머를 위한 핸들
	FTimerHandle SpawnTimerHandle;

	// 발판을 실제로 생성하는 함수
	void SpawnRandomPlatform();
};
