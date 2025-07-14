#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h" //∏∂¡ˆ∏∑

class USphereComponent;

UCLASS()
class SPARTAPROJECT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	

	ABaseItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* Scene; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	UParticleSystem* PickupParticle; //æ∆¿Ã≈€ »πµÊ ¿Ã∆Â∆Æ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	USoundBase* PickupSound; //æ∆¿Ã≈€ »πµÊ ªÁøÓµÂ

	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override; 
	virtual FName GetItemName() const override;


	virtual void DestroyItem();

	
};
