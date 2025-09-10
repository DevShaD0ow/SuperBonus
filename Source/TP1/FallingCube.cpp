#include "FallingCube.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AFallingCube::AFallingCube()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;

	// Charger le cube de base d’Unreal (BasicShapes)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		CubeMesh->SetStaticMesh(CubeAsset.Object);
	}

	// Activer la physique pour qu’il tombe
	CubeMesh->SetSimulatePhysics(true);
}

void AFallingCube::BeginPlay()
{
	Super::BeginPlay();	
}

void AFallingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
