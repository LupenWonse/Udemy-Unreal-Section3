// (c) Ahmet Gencoglu

#include "Section03.h"
#include "OpenDoor.h"


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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	// Find the owning actor
	AActor* Owner = GetOwner();
	// Create a rotator
	FRotator NewRotation = FRotator(0.0f, 90.0f, 0.0f);

	// Set door rotation
	Owner->SetActorRotation(NewRotation);
	GetOwner()->SetActorRelativeRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	// Find the owning actor
	AActor* Owner = GetOwner();
	// Create a rotator
	FRotator NewRotation = FRotator(0.0f, 0.f, 0.0f);

	// Set door rotation
	Owner->SetActorRotation(NewRotation);
	GetOwner()->SetActorRelativeRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	// ...
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		// If the actor that opens this door is in the trigger volume
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (DoorLastOpenTime + DoorCloseDelay < GetWorld()->GetTimeSeconds()) {
		CloseDoor();
	}

}

