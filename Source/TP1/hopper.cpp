// Fill out your copyright notice in the Description page of Project Settings.

#include "hopper.h"
#include "Dropper.h"
#include "Blueprint/UserWidget.h"
#include "FallingCube.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
Ahopper::Ahopper()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void Ahopper::BeginPlay()
{
	Super::BeginPlay();
	nbCubeRec = 0;
	Velocity = FVector::ZeroVector;
}

// Called every frame
void Ahopper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AFallingCube* Target = GetClosestCube();
	if (!Target) return;

	// Calcul de steering uniquement sur Y
	FVector Steering = SeekY(Target->GetActorLocation());

	// Mise à jour vitesse (accélération)
	Velocity += Steering * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);

	// Position avant déplacement
	float OldY = GetActorLocation().Y;

	// Calcul de la nouvelle position
	FVector Dest = GetActorLocation();
	Dest.Y += Velocity.Y * DeltaTime;
	Dest.Y = FMath::Clamp(Dest.Y, MinMap, MaxMap);

	float TargetY = Target->GetActorLocation().Y;

	// Vérifie si on a dépassé la cible
	if ((OldY < TargetY && Dest.Y > TargetY) ||
		(OldY > TargetY && Dest.Y < TargetY) ||
		FMath::Abs(TargetY - Dest.Y) < 5.f)
	{
		Dest.Y = TargetY;
		Velocity.Y = 0.f;
	}

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

	// Réduction proportionnel a la distance 
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

void Ahopper::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AFallingCube::StaticClass()))
	{
		nbCubeRec++;
		OtherActor->Destroy();

		ADropper* MyDropper = Cast<ADropper>(UGameplayStatics::GetActorOfClass(GetWorld(), ADropper::StaticClass()));
		if (MyDropper)
		{
			int Nb = MyDropper->GetNbCube();
			prec = (static_cast<float>(nbCubeRec) / static_cast<float>(Nb)) * 100.0f;
			UE_LOG(LogTemp, Warning, TEXT("Précision : %.2f"), prec);
		}
	}
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
