// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEROOM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindPlayerInput();

	void FindPhysicsHandle();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere)
		float Reach = 120.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* PlayerInput = nullptr;
	bool PhysicsObjectGrabbed = false;

	void Grab();
	FHitResult GetFirstPhysicsBodyInReach();
	void GrabReleased();
	FVector GetGrabReachStart();
	FVector GetGrabReachEnd();
};
