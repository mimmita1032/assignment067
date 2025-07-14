#include "RotatingPlatform.h"
#include "Components/StaticMeshComponent.h"


ARotatingPlatform::ARotatingPlatform()
{
	// Tick함수 사용
	PrimaryActorTick.bCanEverTick = true;

	// 정적 메시 컴포넌트 생성 후 루트 컴포넌트로 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// 기본 회전 속도 설정
	RotationSpeed = 45.0f;
}

// 게임 시작 호출되게 했슴...
void ARotatingPlatform::BeginPlay()
{
	Super::BeginPlay();
}


void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 회전값 계산: Yaw 축 기준으로 회전속도 × 프레임 시간 만큼 회전
	FRotator RotationDelta(0.f, RotationSpeed * DeltaTime, 0.f);

	// 로컬 기준으로 액터에 회전 적용 (Yaw 축 회전)
	/*AddActorLocalRotation(RotationDelta);*/

	AddActorWorldRotation(RotationDelta);//이건 월드기준
}
