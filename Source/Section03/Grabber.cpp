// (c) Ahmet Gencoglu

#include "Section03.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Find, Attach and Setup Input Component
void UGrabber::SetupInputComponent()
{
	/// Get Input controller
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component found"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input Component Missing On: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// Physics handle found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component Missing On: %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// Get the player viewpoint this tick
		FVector playerViewPointLocation;
		FRotator playerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
		// Log out to test
		//UE_LOG(LogTemp, Warning, TEXT("Player location %s , rotation %s"), *(playerViewPointLocation.ToString()), *(playerViewPointRotation.ToString()));

		FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		/// move the object we are holding	
}

FHitResult UGrabber::GetFirstBodyInReach() const
{
	// Get the player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	// Log out to test
	//UE_LOG(LogTemp, Warning, TEXT("Player location %s , rotation %s"), *(playerViewPointLocation.ToString()), *(playerViewPointRotation.ToString()));

	FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	/// Linetrace (Ray-cast) out to reach distance
	FHitResult Hit;

	/// Setup query
	FCollisionQueryParams CollisionParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		playerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		CollisionParameters
	);



	// See what we hit...
	if (Hit.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("Hit object: %s"), *Hit.Actor->GetName());
	}

	return Hit;
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
	FHitResult Hit = GetFirstBodyInReach();
	AActor* actorHit = Hit.GetActor();

	if (actorHit) {
		// Attach phyiscs  handle
		UPrimitiveComponent* componentInReach = GetFirstBodyInReach().GetComponent();

		//GetFirstBodyInReach();
		// Linetrace and see if we reach any actors with physics body collision channel set
		PhysicsHandle->GrabComponent(
			componentInReach,
			NAME_None,
			componentInReach->GetOwner()->GetActorLocation(),
			true // Allow rotation
		);
			// If we hit an object than attach a physics
			
	}
	

}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release"));

	// TODO Release physics handle
	PhysicsHandle->ReleaseComponent();
}
