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

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PHandle = GetPhysicsHandle();
	if(PHandle == nullptr)
    {
		return;
	}

    FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PHandle -> SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());


}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released grabber"));
}

void UGrabber::Grab(){

	UPhysicsHandleComponent* PHandle = GetPhysicsHandle();
	if(PHandle == nullptr)
    {
		return;
	}


    FVector Start = GetComponentLocation();

	FVector End = Start + GetForwardVector() * MaxGrabDistance;

    DrawDebugLine(GetWorld(), Start, End, FColor::Green);

	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
	if (HasHit) {
	
	   DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		AActor *HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("Hit actor: %s"), *HitActor->GetActorNameOrLabel());



	    PHandle -> GrabComponentAtLocationWithRotation(
		HitResult.GetComponent(),
		NAME_None,
		HitResult.ImpactPoint,
		HitResult.GetComponent() -> GetComponentRotation()
		
	   );
	};
    
		
	};



    UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const{

	UPhysicsHandleComponent* Result = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));

		
	}
 
    return Result;

	





