// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/TFHarvestActorBase.h"
#include "TFRegrowingHarvestActorBase.generated.h"

/**
 * 
 */
UCLASS()
class THEFALL_API ATFRegrowingHarvestActorBase : public ATFHarvestActorBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int NumberOfDaysToRegrow = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	int DaysSinceLastHarvest = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	int ItemRegrowthAmount = 1;

	ATFRegrowingHarvestActorBase();
	void ResetHarvest();

protected:
	virtual void BeginPlay();

public:
	void OnDayChange();
};
