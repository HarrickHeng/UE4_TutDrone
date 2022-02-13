// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TutDroneGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TUTDRONE_API ATutDroneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		int32 points;

};
