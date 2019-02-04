// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

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

	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("No Pressure Plate attached to %s"), *GetOwner()->GetName());
	}
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


		if (PressurePlate != nullptr)
			//if (PressurePlate->IsOverlappingActor(ActorThatOpens))
			if (GetTotalmassOfActorsOnPlate() > 30.f) // TODO make into parameter
				OpenDoor();
			else
			{
				float CurrenttimeInSeconds = GetWorld()->GetTimeSeconds();
				bool ShouldCloseDoor = CurrenttimeInSeconds - DoorLastOpenTimeSeconds > DoorCloseDelaySeconds;
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

float UOpenDoor::GetTotalmassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& OverlappingActor : OverlappingActors)
	{
		const auto PrimitiveComponent = OverlappingActor->FindComponentByClass<UPrimitiveComponent>();
		if (PrimitiveComponent != nullptr) {
			TotalMass += PrimitiveComponent->GetMass();
			//UE_LOG(LogTemp, Warning, TEXT("Found %s weighing %f"), *OverlappingActor->GetName(), PrimitiveComponent->GetMass());
		}
	}
	return TotalMass;
}

