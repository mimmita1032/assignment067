#include "RotatingPlatform.h"
#include "Components/StaticMeshComponent.h"


ARotatingPlatform::ARotatingPlatform()
{
	// Tick�Լ� ���
	PrimaryActorTick.bCanEverTick = true;

	// ���� �޽� ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// �⺻ ȸ�� �ӵ� ����
	RotationSpeed = 45.0f;
}

// ���� ���� ȣ��ǰ� �߽�...
void ARotatingPlatform::BeginPlay()
{
	Super::BeginPlay();
}


void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ȸ���� ���: Yaw �� �������� ȸ���ӵ� �� ������ �ð� ��ŭ ȸ��
	FRotator RotationDelta(0.f, RotationSpeed * DeltaTime, 0.f);

	// ���� �������� ���Ϳ� ȸ�� ���� (Yaw �� ȸ��)
	/*AddActorLocalRotation(RotationDelta);*/

	AddActorWorldRotation(RotationDelta);//�̰� �������
}
