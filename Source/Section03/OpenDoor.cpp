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
	// Find the owning actor
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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

