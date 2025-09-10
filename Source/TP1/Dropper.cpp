// Fill out your copyright notice in the Description page of Project Settings.


#include "Dropper.h"

#include "FallingCube.h"

// Sets default values
ADropper::ADropper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADropper::BeginPlay()
{
	Super::BeginPlay();
	Velocity = FVector::ZeroVector;
	ChooseNewTarget();
	
}

//Called every frame
void ADropper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Applique le Seek vers la Target
	FVector Steering = Seek(Target);

	// Mise à jour de la vélocité
	Velocity += Steering * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);

	// Position courante
	FVector CurrentPos = GetActorLocation();

	// Vérifie si on est arrivé à la cible
	if (FMath::Abs(CurrentPos.Y - Target.Y) < 1.f)
	{
		// Spawn juste en dessous du Dropper à SA POSITION ACTUELLE
		FVector SpawnLocation = CurrentPos + FVector(0.f, 0.f, -600.f);
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AFallingCube>(AFallingCube::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		// Ensuite seulement : nouvelle cible
		ChooseNewTarget();
	}
	// Mise à jour de la position (après le test)
	FVector Dest = CurrentPos;
	Dest.Y += Velocity.Y * DeltaTime;
	Dest.Y = FMath::Clamp(Dest.Y, 32850.f, 36600.f);

	SetActorLocation(Dest);
}



FVector ADropper::Seek(const FVector& vTarget)
{
	FVector vDesired = vTarget - GetActorLocation();
	vDesired.Normalize();
	vDesired *= MaxSpeed;

	FVector vSteering = vDesired - Velocity;
	vSteering = vSteering.GetClampedToMaxSize(MaxForce);

	return vSteering;
}

void ADropper::ChooseNewTarget()
{
	FVector Pos = GetActorLocation();
	float RandomY = FMath::FRandRange(32850.f,36600.f ); // C'est les limites de la caméra soit min et max y
	Target = FVector(Pos.X, RandomY, Pos.Z);
}

