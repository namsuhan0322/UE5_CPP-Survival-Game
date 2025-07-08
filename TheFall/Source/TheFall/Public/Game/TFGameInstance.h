// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveActorInterface.h"
#include "TFGameInstance.generated.h"

/**
 * ���� �������� �����Ǵ� ���� ������ GameInstance Ŭ����
 */
UCLASS()
class THEFALL_API UTFGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	// ���� ������ ���͵��� ������ (GUID�� ����)
	UPROPERTY()
	TMap<FGuid, FSaveActorData> SaveableActorData;

	// SaveGame ������Ʈ �ν��Ͻ�
	UPROPERTY()
	class UTFSaveGame* SaveGameObject = nullptr;

	// ���� ���� �̸� (�⺻��: "DEFAULT")
	UPROPERTY()
	FString SaveGameName = TEXT("DEFAULT");

	// ���� �ε�� ���� �̸�
	UPROPERTY()
	FName CurrentlyLoadedLevel = "NONE";

	// �÷��̾� ���� ������
	UPROPERTY()
	FSaveActorData PlayerData;

	// ������ (���ο����� ����)
	UTFGameInstance();

	// ���� ���� ����
	void CreatSaveslot();
	// ���� �����ϴ� ���� ��� ���͵��� �����͸� ����
	void GetherActorData();
	// ����� ���� �����͸� �ҷ�����
	void LoadGame();
	// �÷��̾� ���� �����͸� ����
	void GetherPlayerData();
	// ����� �÷��̾� �����͸� ����
	void SetPlayerData();

public:

	/// ���� ������ ���� �����͸� �߰� (GUID ����)
	UFUNCTION(BlueprintCallable)
	void AddActorData(const FGuid& ActorID, FSaveActorData ActorData);
	/// Ư�� ������ ���� �����͸� ��ȯ
	UFUNCTION(BlueprintCallable)
	FSaveActorData GetActorData(const FGuid& ActorID);

	/// ���߿� ���� ȣ�� �Լ� (����׿�)
	UFUNCTION(BlueprintCallable)
	void DEV_SaveGame();
	/// ���߿� �ε� ȣ�� �Լ� (����׿�)
	UFUNCTION(BlueprintCallable)
	void DEV_LoadGame();
};
