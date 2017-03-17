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
	UE_LOG(LogTemp, Warning, TEXT("Grabber is reporting for duty"));
	// ...

	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// Physics handle found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component Missing On: %s"), *GetOwner()->GetName());
	}

	/// Get Input controller
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component found"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Input Component Missing On: %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player viewpoint this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation,OUT playerViewPointRotation);
	// Log out to test
	//UE_LOG(LogTemp, Warning, TEXT("Player location %s , rotation %s"), *(playerViewPointLocation.ToString()), *(playerViewPointRotation.ToString()));



	FVector LineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	// Draw a red trace in the world
	DrawDebugLine(GetWorld(), playerViewPointLocation, LineTraceEnd, FColor(255, 0, 0),false,0.0f,0,10.f);
	
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
		UE_LOG(LogTemp, Warning, TEXT("Hit object: %s"),*Hit.Actor->GetName());
	}
	
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release"));
}
