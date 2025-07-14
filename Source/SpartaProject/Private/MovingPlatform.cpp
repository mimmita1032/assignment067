#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"

// ������
AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true; // Tick �Լ� ���

	// ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// �̵� �ӵ�, ����, ���� ��ȯ 
	MoveSpeed = 200.f;              // �̵�
	DirectionChangeInterval = 2.5f; // ���� ����
	MoveDirection = 1.f;            // �ʱ� ����: ������ (+X��)
}

//���� ���� �� ����
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// ���� �ð����� FlipDirection �Լ��� �ݺ������� ȣ���ϴ� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(
		DirectionTimerHandle,             // Ÿ�̸� �ڵ�
		this,                             // ȣ�� ���: �ڱ� �ڽ�
		&AMovingPlatform::FlipDirection,  // �̵� ������ �ݴ��!
		DirectionChangeInterval,          // ȣ�� �ֱ�
		true                              // �ݺ� ����
	);
}

// ������ �� Tick���
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ��ġ�� ������
	FVector CurrentLocation = GetActorLocation();

	// �̵� ���� ��� (���� * �ӵ� * DeltaTime)
	FVector MoveOffset = FVector(MoveSpeed * DeltaTime * MoveDirection, 0.f, 0.f);

	// �̵��� ��ġ�� ����
	SetActorLocation(CurrentLocation + MoveOffset);
}

// FlipDirection: ���� ���� ó�� �Լ�
void AMovingPlatform::FlipDirection()
{
	// �̵� ������ �ݴ� �������� ���� (-1 �� 1 �Ǵ� 1 �� -1)
	MoveDirection *= -1.f;
}
