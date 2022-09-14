// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereSpawn.generated.h"

UCLASS()
class SPHERESHOOTERPROJ_API ASphereSpawn : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASphereSpawn();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* SpawnVolume;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Sphere enemy spawn props
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ACharacter* PlayerCharacter;
	
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	int SpawnCount;
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	int SphereCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSphere")
	int WaveCount;
	
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	int DestroyCounter;
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	int DestroyThreshold;
	
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	FVector Loc;
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	FVector LocTemp;
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	int DistanceBetweenSpheres;
	
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	int Radius;
	UPROPERTY(EditAnywhere, Category = "SpawnSphere")
	int MiniRadius;
	
	
	void SpawnSphereEnemy(int& SpawnCounter, int& Rad);
	// Setters for props
	void IncrementDestroyCount();
	void DecrementSphereCount();
	// Getters for props
	int GetDestroyCount();
	int GetDestroyThreshold();
	int GetSphereCount();
	// Random Location of spawning sphere
	void GetRandomLocation(const FVector Location, int Rad); /*FVector*/
};
