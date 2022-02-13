// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(5000.0f, 5000.0f, 200.0f));
}

void AEnemySpawner::SpawnEnemy()
{
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);
	FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxExtent);
	GetWorld()->SpawnActor<AEnemy>(EnemyType, SpawnPoint, FRotator::ZeroRotator);
}

//Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, spawnSpan, true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

