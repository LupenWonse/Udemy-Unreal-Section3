// (c) Ahmet Gencoglu

#include "Section03.h"
#include "OpenDoor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// Find the owning actor
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	// Set door rotation
	//Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	if (Owner->GetActorRotation().Yaw < OpenAngle) {
		Owner->AddActorLocalRotation(FRotator(0.0f, 1.0f, 0.0f));
	}
}

void UOpenDoor::CloseDoor()
{
	// Set door rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > 50.0f) { // TODO Make into a parameter
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (DoorLastOpenTime + DoorCloseDelay < GetWorld()->GetTimeSeconds()) {
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float totalMass = 0.f;

	// Find all overalapping actors
	TArray<AActor*> overlappingActors;
	PressurePlate->GetOverlappingActors(OUT overlappingActors);

	// Iterate over overlapping actors
	for (const auto* actor : overlappingActors) {
		UE_LOG(LogTemp, Warning, TEXT("%s is on pressure plate"), *actor->GetName());
		totalMass += actor->GetRootPrimitiveComponent()->CalculateMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Mass on plate is : %f"), totalMass);


	return totalMass;

}

