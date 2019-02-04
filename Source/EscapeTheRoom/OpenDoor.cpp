// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	if (!IsOpen)
	{
		FRotator NewRotation = GetOwner()->GetActorRotation().Add(0.f, -OpenAngle, 0.f);

		GetOwner()->SetActorRotation(NewRotation);
		IsOpen = true;
	}

	DoorLastOpenTimeSeconds = GetWorld()->GetTimeSeconds();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActorThatOpens != NULL)
		if (PressurePlate != NULL)
			if (PressurePlate->IsOverlappingActor(ActorThatOpens))
				OpenDoor();
			else
			{
				float CurrenttimeInSeconds = GetWorld()->GetTimeSeconds();
				bool ShouldCloseDoor = CurrenttimeInSeconds > DoorLastOpenTimeSeconds + DoorCloseDelaySeconds;
				if (ShouldCloseDoor)
					CloseDoor();
			}
}

void UOpenDoor::CloseDoor()
{
	if (IsOpen)
	{
		FRotator NewRotation = GetOwner()->GetActorRotation().Add(0.f, OpenAngle, 0.f);

		GetOwner()->SetActorRotation(NewRotation);
		IsOpen = false;
	}
}

