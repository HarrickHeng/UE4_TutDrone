// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADrone::ADrone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OutCollision = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	RootComponent = OutCollision;
	OutCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	OutCollision->SetSimulatePhysics(true);
	OutCollision->BodyInstance.bLockYRotation = true;
	OutCollision->BodyInstance.bLockXRotation = true;

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OutCollision);
	Paddle1 = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2 = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3 = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4 = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));
	Paddle1->SetupAttachment(Mesh, TEXT("Paddle1"));
	Paddle2->SetupAttachment(Mesh, TEXT("Paddle2"));
	Paddle3->SetupAttachment(Mesh, TEXT("Paddle3"));
	Paddle4->SetupAttachment(Mesh, TEXT("Paddle4"));
	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);

	UpThruster = CreateAbstractDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
	UpThruster->SetupAttachment(RootComponent);
	UpThruster->ThrustStrength = 980.0f;
	UpThruster->SetAutoActivate(true);
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));

	ForwardThruster = CreateAbstractDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
	ForwardThruster->SetupAttachment(RootComponent);
	ForwardThruster->ThrustStrength = 0.0f;
	ForwardThruster->SetAutoActivate(true);
	ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()));
}

void ADrone::DoFire()
{
	FTransform f_point = Mesh->GetSocketTransform(TEXT("fireSocket"));
	GetWorld()->SpawnActor<AMissle>(Bullet, f_point);
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetInputAxisValue(TEXT("Lift")) == 0.0f)
	{
		UpThruster->ThrustStrength = 980.0f;
	}
	if (GetInputAxisValue(TEXT("Forward")) == 0.0f)
	{
		ForwardThruster->ThrustStrength = 0.0f;

		float currentPitch = Mesh->GetRelativeRotation().Pitch;
		if (currentPitch != 0.0f)
		{
			Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime * 2.0f, .0f, .0f));
			if (FMath::Abs(currentPitch) <= KINDA_SMALL_NUMBER)
			{
				Mesh->SetRelativeRotation(FRotator(.0f, .0f, .0f));
			}
		}
	}
	RotatePaddle(DeltaTime);
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ADrone::DoFire);
}

void ADrone::Lift(float val)
{
	UpThruster->ThrustStrength += val * LiftAcc * GetWorld()->DeltaTimeSeconds;
	UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength, -LiftMaxAcc, LiftMaxAcc);
}

void ADrone::Forward(float val)
{
	ForwardThruster->ThrustStrength += val * ForwardAcc * GetWorld()->DeltaTimeSeconds;
	ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardMaxAcc, ForwardMaxAcc);

	float currentPitch = Mesh->GetRelativeRotation().Pitch;
	if ((-val * currentPitch) < 30.0f)
	{
		Mesh->AddRelativeRotation(FRotator(-val * 100.f * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f));
	}
}

void ADrone::Turn(float val)
{
	OutCollision->AddTorqueInDegrees(-this->GetActorUpVector() * val * TurnStrength);
}

void ADrone::RotatePaddle(float Delta)
{
	for (auto paddle : Paddles)
	{
		paddle->AddRelativeRotation(FRotator(.0f, Delta * PaddleRotateSpeed, .0f));
	}
}

