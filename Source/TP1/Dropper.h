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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Steering")float MaxSpeed=2000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Steering")float MaxForce=1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Spawning")float MinSpawnInterval=0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Spawning")float MaxSpawnInterval=3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category="Terrain")float MinMap=32320.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Terrain")float MaxMap=35610.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Terrain")int NBMaxCube=50;
	UFUNCTION(BlueprintCallable, Category="Dropper")
	int GetNbCube() const { return nbCube; }

private:
	FTimerHandle SpawnTimerHandle;
	FVector Velocity;
	FVector Target;
	FVector Seek(const FVector& vTarget);
	static int nbCube;

	void ChooseNewTarget();
	void SpawnCube();
	void StartRandomSpawnTimer();
};
