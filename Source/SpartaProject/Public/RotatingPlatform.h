#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingPlatform.generated.h"

UCLASS()
class SPARTAPROJECT_API ARotatingPlatform : public AActor
{
	GENERATED_BODY()

public:
	ARotatingPlatform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationSpeed;
};
