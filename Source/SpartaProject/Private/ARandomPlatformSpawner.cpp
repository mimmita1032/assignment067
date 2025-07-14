#include "ARandomPlatformSpawner.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// ������
ARandomPlatformSpawner::ARandomPlatformSpawner()
{
	PrimaryActorTick.bCanEverTick = false; // �� Ŭ������ �� ������ ó�� �ʿ� ����

	// �⺻ ���� ���� ���� 
	SpawnInterval = 3.f;

	// ���� ���� �⺻�� ���� (X, Y ��ǥ ���� ����)
	SpawnMin = FVector2D(-500.f, -500.f);
	SpawnMax = FVector2D(500.f, 500.f);
}

// ���� ���� �� ȣ��ǰ�
void ARandomPlatformSpawner::BeginPlay()
{
	Super::BeginPlay();

	// �ݺ� Ÿ�̸� ����... ���� �ð����� SpawnRandomPlatform ȣ��
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,                  // Ÿ�̸� �ڵ�
		this,                              // ȣ�� ��� (�ڱ� �ڽ�)
		&ARandomPlatformSpawner::SpawnRandomPlatform, // ������ �Լ�
		SpawnInterval,                     // ����
		true                               // �ݺ� ����: true 
	);
}

// ���� ���� ����
void ARandomPlatformSpawner::SpawnRandomPlatform()
{
	// 50% Ȯ���� ȸ�� or �̵� ���� Ŭ���� ����
	TSubclassOf<AActor> PlatformToSpawn = FMath::RandBool() ? RotatingPlatformClass : MovingPlatformClass;

	// Ŭ������ �������� ���� ��� ���� �ߴ�
	if (!PlatformToSpawn) return;

	// X, Y ��ǥ�� �����ϰ� ����
	float RandX = FMath::FRandRange(SpawnMin.X, SpawnMax.X);
	float RandY = FMath::FRandRange(SpawnMin.Y, SpawnMax.Y);

	// Z ��ǥ�� �� ������ ������ Z ��ġ ��� (���� ����) �ٲܿ���...
	float SpawnZ = GetActorLocation().Z;

	// ���� ���� ��ġ ���
	FVector SpawnLocation(RandX, RandY, SpawnZ);

	// ���� ���� (ȸ���� 0���� ����)
	GetWorld()->SpawnActor<AActor>(PlatformToSpawn, SpawnLocation, FRotator::ZeroRotator);
}
