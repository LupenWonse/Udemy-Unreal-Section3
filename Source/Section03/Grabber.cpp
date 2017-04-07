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
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component Missing On: %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle is attached
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent) {
		/// move the object we are holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FHitResult UGrabber::GetFirstBodyInReach()
{
	/// Linetrace (Ray-cast) out to reach distance
	FHitResult HitResult;

	/// Setup query
	FCollisionQueryParams CollisionParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		CollisionParameters
	);
	return HitResult;
}

FVector UGrabber::GetReachLineStart() {
	// Get the player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	return playerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd() {
	// Get the player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
}

void UGrabber::Grab() {
	if (!PhysicsHandle) { return; }
	
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
	FHitResult Hit = GetFirstBodyInReach();
	AActor* actorHit = Hit.GetActor();

	if (actorHit) {
		// Attach phyiscs  handle
		UPrimitiveComponent* componentInReach = GetFirstBodyInReach().GetComponent();

		// Linetrace and see if we reach any actors with physics body collision channel set
		PhysicsHandle->GrabComponent(
			componentInReach,
			NAME_None, // No bones needed
			componentInReach->GetOwner()->GetActorLocation(),
			true // Allow rotation
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	if (!PhysicsHandle) { return; }
	// TODO Release physics handle
	PhysicsHandle->ReleaseComponent();
}
