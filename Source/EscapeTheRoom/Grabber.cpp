// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	FindPlayerInput();
}

void UGrabber::FindPlayerInput()
{
	PlayerInput = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PlayerInput)
	{
		PlayerInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		PlayerInput->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
	}
	else
		UE_LOG(LogTemp, Error, TEXT(""), *GetOwner()->GetName());
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("No Phsycis Handle attached to %s"), *GetOwner()->GetName());
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		FVector Position;
		FRotator ViewDirection;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Position, ViewDirection);
		FVector LineTraceEnd = Position + ViewDirection.Vector() * Reach;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbed"));

	auto HitResult = GetFirstPhysicsBodyInReach();

	if (HitResult.GetActor() != NULL)
	{
		auto ComponentToGrab = HitResult.GetComponent();

		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			HitResult.GetActor()->GetActorLocation(),
			true
		);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector Position;
	FRotator ViewDirection;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Position, ViewDirection);
	FVector LineTraceEnd = Position + ViewDirection.Vector() * Reach;

	//DrawDebugLine(
	//	GetWorld(),
	//	Position,
	//	LineTraceEnd,
	//	FColor(0xff, 0, 0),
	//	false,
	//	0.f,
	//	0.f,
	//	10.f
	//);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		LineTraceHit,
		Position,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	if (LineTraceHit.GetActor() != NULL)
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *LineTraceHit.GetActor()->GetName());

	return LineTraceHit;
}

void UGrabber::GrabReleased() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbed Released"));

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}