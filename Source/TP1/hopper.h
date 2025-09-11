#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "hopper.generated.h"

// Forward declaration pour éviter les erreurs de compilation
class AFallingCube;

UCLASS()
class TP1_API Ahopper : public AActor
{
	GENERATED_BODY()
	
public:	
	Ahopper();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Steering")float MaxSpeed = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Steering")float MaxForce = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Terrain")float MinMap = 32130.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Terrain")float MaxMap = 35750.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Precision")float prec = 0.0f;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	FVector Velocity;
	int nbCubeRec;
	FVector SeekY(const FVector& vTarget);

	AFallingCube* GetClosestCube();
};
