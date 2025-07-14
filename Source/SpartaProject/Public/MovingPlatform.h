#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class SPARTAPROJECT_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DirectionChangeInterval;

	FTimerHandle DirectionTimerHandle;

	float MoveDirection;

	void FlipDirection();// ���� ���� �Լ� - Ÿ�̸ӿ� ���� ȣ���
};
