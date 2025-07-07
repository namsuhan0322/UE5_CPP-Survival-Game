// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveActorInterface.generated.h"

USTRUCT(BlueprintType)
struct FSaveComponentData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* ComponentClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> RawData;
};

USTRUCT(BlueprintType)
struct FSaveActorData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSaveComponentData> ComponentData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool WasSpawned = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* ActorClass;
};

UINTERFACE(MinimalAPI)
class USaveActorInterface : public UInterface
{
	GENERATED_BODY()
};


class THEFALL_API ISaveActorInterface
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintNativeEvent)
	FGuid GetActorSaveID();
	virtual FGuid GetActorSaveID_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	FSaveActorData GetSaveData();
	virtual FSaveActorData GetSaveData_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	FSaveComponentData GetComponentSaveData();
	virtual FSaveComponentData GetComponentSaveData_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void SetComponentSaveData(FSaveComponentData Data);
	virtual void SetComponentSaveData_Implementation(FSaveComponentData Data);
};
