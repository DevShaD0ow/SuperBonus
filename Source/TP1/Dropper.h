// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h" // pour FTimerHandle
#include "Dropper.generated.h"

UCLASS()
class TP1_API ADropper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropper();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Steering")float MaxSpeed=600.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Steering")float MaxForce=200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Spawning")float MinSpawnInterval=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Spawning")float MaxSpawnInterval=2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Terrain")float MinMap=32130.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Terrain")float MaxMap=35750.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Terrain")int NBMaxCube=4;
	
private:
	FTimerHandle SpawnTimerHandle;
	FVector Velocity;
	FVector Target;
	INT nbCube;
	FVector Seek(const FVector& vTarget);
	void ChooseNewTarget();
	void SpawnCube();
	void StartRandomSpawnTimer();
};
