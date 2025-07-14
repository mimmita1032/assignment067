#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"

// 생성자
AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true; // Tick 함수 사용

	// 컴포넌트 생성 및 루트 컴포넌트로 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// 이동 속도, 방향, 방향 전환 
	MoveSpeed = 200.f;              // 이동
	DirectionChangeInterval = 2.5f; // 방향 반전
	MoveDirection = 1.f;            // 초기 방향: 정방향 (+X축)
}

//게임 시작 시 실행
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// 일정 시간마다 FlipDirection 함수를 반복적으로 호출하는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		DirectionTimerHandle,             // 타이머 핸들
		this,                             // 호출 대상: 자기 자신
		&AMovingPlatform::FlipDirection,  // 이동 방향을 반대로!
		DirectionChangeInterval,          // 호출 주기
		true                              // 반복 실행
	);
}

// 움직일 때 Tick사용
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 위치를 가져옴
	FVector CurrentLocation = GetActorLocation();

	// 이동 벡터 계산 (방향 * 속도 * DeltaTime)
	FVector MoveOffset = FVector(MoveSpeed * DeltaTime * MoveDirection, 0.f, 0.f);

	// 이동한 위치로 갱신
	SetActorLocation(CurrentLocation + MoveOffset);
}

// FlipDirection: 방향 반전 처리 함수
void AMovingPlatform::FlipDirection()
{
	// 이동 방향을 반대 방향으로 변경 (-1 → 1 또는 1 → -1)
	MoveDirection *= -1.f;
}
