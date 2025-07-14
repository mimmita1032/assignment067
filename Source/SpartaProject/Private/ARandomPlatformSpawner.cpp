#include "ARandomPlatformSpawner.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// 생성자
ARandomPlatformSpawner::ARandomPlatformSpawner()
{
	PrimaryActorTick.bCanEverTick = false; // 이 클래스는 매 프레임 처리 필요 없음

	// 기본 생성 간격 설정 
	SpawnInterval = 3.f;

	// 생성 범위 기본값 설정 (X, Y 좌표 랜덤 범위)
	SpawnMin = FVector2D(-500.f, -500.f);
	SpawnMax = FVector2D(500.f, 500.f);
}

// 게임 시작 시 호출되게
void ARandomPlatformSpawner::BeginPlay()
{
	Super::BeginPlay();

	// 반복 타이머 설정... 일정 시간마다 SpawnRandomPlatform 호출
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,                  // 타이머 핸들
		this,                              // 호출 대상 (자기 자신)
		&ARandomPlatformSpawner::SpawnRandomPlatform, // 실행할 함수
		SpawnInterval,                     // 간격
		true                               // 반복 여부: true 
	);
}

// 발판 생성 로직
void ARandomPlatformSpawner::SpawnRandomPlatform()
{
	// 50% 확률로 회전 or 이동 발판 클래스 선택
	TSubclassOf<AActor> PlatformToSpawn = FMath::RandBool() ? RotatingPlatformClass : MovingPlatformClass;

	// 클래스가 설정되지 않은 경우 생성 중단
	if (!PlatformToSpawn) return;

	// X, Y 좌표를 랜덤하게 선택
	float RandX = FMath::FRandRange(SpawnMin.X, SpawnMax.X);
	float RandY = FMath::FRandRange(SpawnMin.Y, SpawnMax.Y);

	// Z 좌표는 이 스포너 액터의 Z 위치 사용 (기준 높이) 바꿀예정...
	float SpawnZ = GetActorLocation().Z;

	// 최종 생성 위치 계산
	FVector SpawnLocation(RandX, RandY, SpawnZ);

	// 발판 생성 (회전은 0으로 고정)
	GetWorld()->SpawnActor<AActor>(PlatformToSpawn, SpawnLocation, FRotator::ZeroRotator);
}
