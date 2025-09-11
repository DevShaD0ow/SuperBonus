// Fill out your copyright notice in the Description page of Project Settings.


#include "hopper.h"
#include "FallingCube.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
Ahopper::Ahopper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Ahopper::BeginPlay()
{
	Super::BeginPlay();
	Velocity = FVector::ZeroVector;
}

// Called every frame
void Ahopper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AFallingCube* Target=GetClosestCube();
	if (!Target)return;

	// Calcul de steering uniquement sur Y
	FVector Steering = SeekY(Target->GetActorLocation());

	// Mise à jour vitesse
	Velocity += Steering * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);

	// Mise à jour position uniquement en Y
	FVector Dest = GetActorLocation();
	Dest.Y += Velocity.Y * DeltaTime;
	Dest.Y = FMath::Clamp(Dest.Y, MinMap, MaxMap);

	SetActorLocation(Dest);
}

FVector Ahopper::SeekY(const FVector& vTarget)
{
	FVector CurrentPos = GetActorLocation();
	float DesiredY = vTarget.Y - CurrentPos.Y;

	// Direction uniquement sur Y
	FVector vDesired(0.f, DesiredY, 0.f);

	// Distance absolue pour ajuster la vitesse
	float Distance = FMath::Abs(DesiredY);

	// Normalisation
	if (!vDesired.IsNearlyZero())
	{
		vDesired.Normalize();
	}

	// Si loin → pleine vitesse
	// Si proche (<500 unités par ex) → réduire proportionnellement
	float Speed = MaxSpeed;
	if (Distance < 500.f)
	{
		Speed = MaxSpeed * (Distance / 500.f);
	}

	vDesired *= Speed;

	// Steering force
	FVector vSteering = vDesired - Velocity;
	vSteering = vSteering.GetClampedToMaxSize(MaxForce);

	return vSteering;
}


AFallingCube* Ahopper::GetClosestCube()
{
	TArray<AActor*> FoundCubes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingCube::StaticClass(), FoundCubes);

	AFallingCube* Closest = nullptr;
	float MinDist = FLT_MAX;

	for (AActor* Actor : FoundCubes)
	{
		float Dist = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
		if (Dist < MinDist)
		{
			MinDist = Dist;
			Closest = Cast<AFallingCube>(Actor);
		}
	}

	return Closest;
}

