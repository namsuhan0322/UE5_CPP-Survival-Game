// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveActorInterface.generated.h"

/// 컴포넌트 저장용 데이터 구조체
USTRUCT(BlueprintType)
struct FSaveComponentData
{
	GENERATED_USTRUCT_BODY()

public:
	// 컴포넌트의 직렬화된 바이너리 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;

	// 컴포넌트의 클래스 타입 (로드 시 생성용)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* ComponentClass;

	// 추가적인 문자열 데이터 (텍스트 형태 저장용)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> RawData;
};

/// 액터 저장용 데이터 구조체
USTRUCT(BlueprintType)
struct FSaveActorData
{
	GENERATED_USTRUCT_BODY()

public:
	// 액터의 위치, 회전, 스케일 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;

	// 액터의 직렬화된 바이너리 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;

	// 액터가 보유한 컴포넌트들의 저장 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSaveComponentData> ComponentData;

	// 해당 액터가 런타임에 스폰된 것인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool WasSpawned = false;

	// 액터의 클래스 정보 (로드 시 재생성용)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* ActorClass;
};

/// 액터 저장/불러오기를 위한 인터페이스 선언 (Blueprint에서도 사용 가능)
UINTERFACE(MinimalAPI)
class USaveActorInterface : public UInterface
{
	GENERATED_BODY()
};

/// 실제 저장 기능을 구현하는 인터페이스 클래스
class THEFALL_API ISaveActorInterface
{
	GENERATED_BODY()

public:

	/// 저장용 고유 식별자 반환
	UFUNCTION(BlueprintNativeEvent)
	FGuid GetActorSaveID();
	virtual FGuid GetActorSaveID_Implementation();

	/// 저장용 고유 식별자 설정
	UFUNCTION(BlueprintNativeEvent)
	void SetActorGUID(const FGuid& NewGuid);
	virtual void SetActorGUID_Implementation(const FGuid& NewGuid);

	/// 액터 전체 저장 데이터 반환
	UFUNCTION(BlueprintNativeEvent)
	FSaveActorData GetSaveData();
	virtual FSaveActorData GetSaveData_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateFromSave();
	void UpdateFromSave_Implementation();

	/// 액터의 컴포넌트 저장 데이터 반환
	UFUNCTION(BlueprintNativeEvent)
	FSaveComponentData GetComponentSaveData();
	virtual FSaveComponentData GetComponentSaveData_Implementation();

	/// 컴포넌트 저장 데이터를 불러와 적용
	UFUNCTION(BlueprintNativeEvent)
	void SetComponentSaveData(FSaveComponentData Data);
	virtual void SetComponentSaveData_Implementation(FSaveComponentData Data);
};