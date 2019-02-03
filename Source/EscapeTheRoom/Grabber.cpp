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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Position;
	FRotator ViewDirection;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Position, ViewDirection);

	//UE_LOG(LogTemp, Log, TEXT("Player position %s, looking at %s."),
	//	*Position.ToString(),
	//	*ViewDirection.ToString()
	//);


	FVector LineTraceEnd = Position + ViewDirection.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		Position,
		LineTraceEnd,
		FColor(0xff, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);
}

