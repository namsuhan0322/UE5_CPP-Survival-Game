// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveActorInterface.generated.h"

/// ������Ʈ ����� ������ ����ü
USTRUCT(BlueprintType)
struct FSaveComponentData
{
	GENERATED_USTRUCT_BODY()

public:
	// ������Ʈ�� ����ȭ�� ���̳ʸ� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;

	// ������Ʈ�� Ŭ���� Ÿ�� (�ε� �� ������)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* ComponentClass;

	// �߰����� ���ڿ� ������ (�ؽ�Ʈ ���� �����)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> RawData;
};

/// ���� ����� ������ ����ü
USTRUCT(BlueprintType)
struct FSaveActorData
{
	GENERATED_USTRUCT_BODY()

public:
	// ������ ��ġ, ȸ��, ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;

	// ������ ����ȭ�� ���̳ʸ� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;

	// ���Ͱ� ������ ������Ʈ���� ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSaveComponentData> ComponentData;

	// �ش� ���Ͱ� ��Ÿ�ӿ� ������ ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool WasSpawned = false;

	// ������ Ŭ���� ���� (�ε� �� �������)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* ActorClass;
};

/// ���� ����/�ҷ����⸦ ���� �������̽� ���� (Blueprint������ ��� ����)
UINTERFACE(MinimalAPI)
class USaveActorInterface : public UInterface
{
	GENERATED_BODY()
};

/// ���� ���� ����� �����ϴ� �������̽� Ŭ����
class THEFALL_API ISaveActorInterface
{
	GENERATED_BODY()

public:

	/// ����� ���� �ĺ��� ��ȯ
	UFUNCTION(BlueprintNativeEvent)
	FGuid GetActorSaveID();
	virtual FGuid GetActorSaveID_Implementation();

	/// ����� ���� �ĺ��� ����
	UFUNCTION(BlueprintNativeEvent)
	void SetActorGUID(const FGuid& NewGuid);
	virtual void SetActorGUID_Implementation(const FGuid& NewGuid);

	/// ���� ��ü ���� ������ ��ȯ
	UFUNCTION(BlueprintNativeEvent)
	FSaveActorData GetSaveData();
	virtual FSaveActorData GetSaveData_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateFromSave();
	void UpdateFromSave_Implementation();

	/// ������ ������Ʈ ���� ������ ��ȯ
	UFUNCTION(BlueprintNativeEvent)
	FSaveComponentData GetComponentSaveData();
	virtual FSaveComponentData GetComponentSaveData_Implementation();

	/// ������Ʈ ���� �����͸� �ҷ��� ����
	UFUNCTION(BlueprintNativeEvent)
	void SetComponentSaveData(FSaveComponentData Data);
	virtual void SetComponentSaveData_Implementation(FSaveComponentData Data);
};