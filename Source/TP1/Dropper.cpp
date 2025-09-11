// Fill out your copyright notice in the Description page of Project Settings.


#include "Dropper.h"
#include "TimerManager.h" // pour FTimerHandle
#include "FallingCube.h"

int ADropper::nbCube = 0;
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
	ADropper::nbCube = 0;
	Velocity = FVector::ZeroVector;
	ChooseNewTarget();
	StartRandomSpawnTimer();
	
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
	if (FMath::Abs(CurrentPos.Y - Target.Y) < 10.f)
	{
		// Ensuite seulement : nouvelle cible
		ChooseNewTarget();
	}
	// Mise à jour de la position (après le test)
	FVector Dest = CurrentPos;
	Dest.Y += Velocity.Y * DeltaTime;
	Dest.Y = FMath::Clamp(Dest.Y, MinMap, MaxMap);

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
	float RandomY = FMath::FRandRange(MinMap,MaxMap ); // C'est les limites de la caméra soit min et max y
	Target = FVector(Pos.X, RandomY, Pos.Z);
}

void ADropper::StartRandomSpawnTimer()
{
	float RandomDelay = FMath::FRandRange(MinSpawnInterval, MaxSpawnInterval);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,this,&ADropper::SpawnCube,RandomDelay,false);
}

void ADropper::SpawnCube()
{
	if (ADropper::nbCube!=NBMaxCube)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, -100.f);
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AFallingCube>(AFallingCube::StaticClass(),SpawnLocation,FRotator::ZeroRotator,SpawnParams);
		ADropper::nbCube++;
		StartRandomSpawnTimer();
	}
}


