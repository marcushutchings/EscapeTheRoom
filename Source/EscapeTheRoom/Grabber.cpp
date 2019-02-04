// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

#define OUT

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

	if (PhysicsObjectGrabbed)
	{
		PhysicsHandle->SetTargetLocation(GetGrabReachEnd());
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
		PhysicsObjectGrabbed = true;
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
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
		GetGrabReachStart(),
		GetGrabReachEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	if (LineTraceHit.GetActor() != NULL)
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *LineTraceHit.GetActor()->GetName());

	return LineTraceHit;
}

void UGrabber::GrabReleased()
{
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
		PhysicsObjectGrabbed = false;
	}
}

FVector UGrabber::GetGrabReachStart()
{
	FVector Position;
	FRotator ViewDirection;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Position, OUT ViewDirection);
	return Position;
}

FVector UGrabber::GetGrabReachEnd()
{
	FVector Position;
	FRotator ViewDirection;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Position, OUT ViewDirection);
	return Position + ViewDirection.Vector() * Reach;
}