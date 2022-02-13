// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missle.generated.h"

UCLASS()
class TUTDRONE_API AMissle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissle();

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* MovementComp;
	UPROPERTY(EditAnywhere)
		class USoundBase* MissleSound;

	UFUNCTION()
		void OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
