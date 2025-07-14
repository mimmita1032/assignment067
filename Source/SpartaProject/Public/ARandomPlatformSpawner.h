#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARandomPlatformSpawner.generated.h"

// �� Ŭ������ ���� �ð����� ������ ��ġ�� ������ �����ϴ� ������ ������ �Ѵ�.
UCLASS()
class SPARTAPROJECT_API ARandomPlatformSpawner : public AActor
{
	GENERATED_BODY()

public:
	// ������
	ARandomPlatformSpawner();

protected:
	// ���� ���� �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() override;

private:
	// ���� ���� �ֱ� (�� ����, ��: 3�ʸ��� ����)
	UPROPERTY(EditAnywhere)
	float SpawnInterval;

	// ������ X, Y ��ġ�� �ּ� ��ǥ
	UPROPERTY(EditAnywhere)
	FVector2D SpawnMin;

	// ������ X, Y ��ġ�� �ִ� ��ǥ
	UPROPERTY(EditAnywhere)
	FVector2D SpawnMax;

	// ������ ȸ�� ���� Ŭ���� (�������Ʈ���� ����)
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RotatingPlatformClass;

	// ������ �̵� ���� Ŭ���� (�������Ʈ���� ����)
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MovingPlatformClass;

	// �ݺ� Ÿ�̸Ӹ� ���� �ڵ�
	FTimerHandle SpawnTimerHandle;

	// ������ ������ �����ϴ� �Լ�
	void SpawnRandomPlatform();
};
