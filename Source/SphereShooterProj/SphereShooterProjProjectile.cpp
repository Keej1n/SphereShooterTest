// Copyright Epic Games, Inc. All Rights Reserved.

#include "SphereShooterProjProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SphereEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "SphereSpawn.h"

ASphereShooterProjProjectile::ASphereShooterProjProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASphereShooterProjProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	
}

void ASphereShooterProjProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	ASphereEnemy* Enemy = Cast<ASphereEnemy>(OtherActor);
	// For access to SphereSpawn properties
	AActor* FoundSphereSpawn = UGameplayStatics::GetActorOfClass(GetWorld(), 
			ASphereSpawn::StaticClass());
	SphereSpawn = Cast<ASphereSpawn>(FoundSphereSpawn);
	if (Enemy)
	{
		// Check if we destroy sphere enemies in small radius (1500)
		if (FVector::Distance(Enemy->GetActorLocation(), RootComponent->GetComponentLocation()) <= SphereSpawn->MiniRadius) 
		{
			Enemy->Destroy();
			SphereSpawn->IncrementDestroyCount();
			SphereSpawn->DecrementSphereCount();
		}
		else
		{
			Enemy->Destroy();
			SphereSpawn->DecrementSphereCount();
		}
		if (SphereSpawn->GetDestroyCount() == SphereSpawn->GetDestroyThreshold())
		{
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASphereEnemy::StaticClass(), Actors);
			if (Actors.Num() == 0)
			{
				SphereSpawn->SphereCount = 0;
				SphereSpawn->SpawnSphereEnemy(SphereSpawn->SpawnCount, SphereSpawn->Radius);
			}
			else
			{
				for(AActor* SphereEnemy :Actors)
				{
					SphereEnemy->Destroy();
				}
				SphereSpawn->SphereCount = 0;
				SphereSpawn->SpawnSphereEnemy(SphereSpawn->SpawnCount, SphereSpawn->Radius);
			}
			
		}
		else if (SphereSpawn->GetSphereCount() == 0)
		{
			SphereSpawn->SpawnSphereEnemy(SphereSpawn->SpawnCount, SphereSpawn->Radius);
		}
	}
}