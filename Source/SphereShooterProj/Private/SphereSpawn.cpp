// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereSpawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "SphereEnemy.h"
#include "NavigationSystem.h"
#include "Settings/EditorProjectSettings.h"

// Sets default values
ASphereSpawn::ASphereSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASphereSpawn::BeginPlay()
{
	Super::BeginPlay();
	WaveCount = 0;
	SpawnCount = 15; // Quantity of Spheres to spawn
	SphereCount = 0; // Quantity of spawned Spheres
	Radius = 2000; // Radius of spawn
	MiniRadius = 1500; // Threshold Radius which allows to complete wave when DestroyCounter == DestroyThreshold (DestroyThreshold amount of Spheres at <=1500 Radius destroyed) 
	DestroyCounter = 0; // Counter of Destroyed Spheres at <=1500 Radius
	DestroyThreshold = 10; // Max value of required amount of Spheres needed to be destroyed to end current wave and start the next
	DistanceBetweenSpheres = 80; // Distance between spheres to spawn
	SpawnSphereEnemy(SpawnCount, Radius);
}

void ASphereSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/**if(SphereCount == 0)
	{
		SpawnSphereEnemy(SpawnCount, Radius, SphereCount);
	}**/
}
// Helps to count Spheres with <=1500 Range
void ASphereSpawn::IncrementDestroyCount()
{
	DestroyCounter++;
}
// Helps to count Spheres when destroyed
void ASphereSpawn::DecrementSphereCount()
{
	--SphereCount;
}
// Getter. Need for additional checking that all Spheres was destroyed. In case when more than 10 spheres destroyed at more than 1500 range
int ASphereSpawn::GetSphereCount()
{
	return SphereCount;
}
// Getter that needed for checking that 10 Spheres at less than 1500 range was destroyed
int ASphereSpawn::GetDestroyCount()
{
	return DestroyCounter;
}
// Getter needed for comparison with DestroyCounter. 
int ASphereSpawn::GetDestroyThreshold()
{
	return DestroyThreshold;
}
// Need to generate a point inside a Sphere with Radius (1500 or 2000 + 0.05 for every iteration of spawn wave)
void ASphereSpawn::GetRandomLocation(const FVector Location, int Rad) /*FVector*/
{
	// First Variant - Incorrect but works. 
	srand(time(NULL));
	Loc.X = float(Location.X + rand() % int(Rad));
	Loc.Y = float(Location.Y + rand() % int(Rad - Location.X));
	Loc.Z = float(Location.Z + rand() % int(Rad - Location.X - Location.Y));
	
	// Second Variant - Correct but doesn't start
	/*bool NavResult = false;
	FNavLocation ResultLoc;
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	while(NavResult)
	{
		NavResult = NavSystem->GetRandomReachablePointInRadius(Location, Rad, ResultLoc);
	}
	return ResultLoc.Location;*/
	
	// Third Variant - Correct but doesn't start
	/*srand(time(NULL));
	const double Angle1 = 2 * PI * rand();
	const double Angle2 = sqrt(cos(2*rand()));
	
	Loc.X = Location.X + Rad * cos(Angle2);
	Loc.Y = Location.Y + Rad * sin(Angle1);
	Loc.Z = Location.Z + cos(Angle1);*/
}
// Spawn Method for EnemySpheres
void ASphereSpawn::SpawnSphereEnemy(int &SpawnC, int &Rad)
{
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0); // To get ActorLocation
	const FVector Location = PlayerCharacter->GetActorLocation();
	const FRotator Rotation = PlayerCharacter->GetActorRotation();
	while (true)
	{
		if (SphereCount == 0)
		{
			++SphereCount;
			// Generate location of sphere which are at the <=1500 distance
			GetRandomLocation(Location, MiniRadius);
			GetWorld()->SpawnActor<ASphereEnemy>(Loc, Rotation);
			LocTemp = Loc; // Temporary Location Vector that needed to check distance between previous and current spheres. Sorry for that. It is not what needed in task. Just noticed too late :(
		}
		else if (SphereCount > 0 && SphereCount < DestroyThreshold)
		{
			GetRandomLocation(Location, MiniRadius);
			if (FVector::Distance(LocTemp, Loc) >= DistanceBetweenSpheres) // Check the distance between previous and current spheres
				{
					++SphereCount;
					GetWorld()->SpawnActor<ASphereEnemy>(Loc, Rotation);
					LocTemp = Loc;
				}
			
		}
		else if (SphereCount >= DestroyThreshold && SphereCount < SpawnC)
		{
			// Generate location of sphere which are at the <=2000 distance
			GetRandomLocation(Location, Rad);
			if (FVector::Distance(LocTemp, Loc) >= DistanceBetweenSpheres)
			{
				++SphereCount;
				GetWorld()->SpawnActor<ASphereEnemy>(Loc, Rotation);
				
			}
			LocTemp = Loc;
		}
		// Change the Count and Radius values after every spawn wave
		else if (SphereCount == SpawnC)
		{
			SpawnC += 0.1 * SpawnC;
			Rad += 0.05 * Rad;
			++WaveCount;
			break;
		}
	}
}
