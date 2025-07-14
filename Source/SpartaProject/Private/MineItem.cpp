#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 50.0f;
	ItemType = "Mine";
	bHasExploded = false;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);

 }

void AMineItem::ActivateItem(AActor* Activator)
{

	if(bHasExploded)
	{
		return; // 이미 폭발한 경우, 아무 작업도 하지 않음
	}
	Super::ActivateItem(Activator); // 부모 클래스의 ActivateItem 호출

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle, 
		this, 
		&AMineItem::Explode, 
		ExplosionDelay, 
		false
	);
	bHasExploded = true; // 폭발 상태로 설정
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;
	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), 
			ExplosionParticle, 
			GetActorLocation(), 
			GetActorRotation(),
			false
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound, 
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor, 
				ExplosionDamage, 
				nullptr,
				this, 
				UDamageType::StaticClass()
			);

		}
	}
	DestroyItem(); 

	if (Particle)
	{
		FTimerHandle DestroyParicleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParicleTimerHandle, 
			[Particle]()
			{
				if (Particle)
				{
					Particle->DestroyComponent();
				}
			}, 
			2.0f, 
			false
		);
	}
}

