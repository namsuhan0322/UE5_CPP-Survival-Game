// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Interface/SaveActorInterface.h"
#include "TFSaveGame.generated.h"

/// ���� ���� �����͸� �����ϴ� SaveGame Ŭ����
UCLASS()
class THEFALL_API UTFSaveGame : public USaveGame
{
	GENERATED_BODY()

private:

	// ���� �� ���� ������ ���͵��� ������ (GUID ������� ����)
	UPROPERTY()
	TMap<FGuid, FSaveActorData> SaveableActorData;
	// ���� �ε�� ���� �̸� (�ҷ����� �� ����� ���� ���п�)
	UPROPERTY()
	FName CurrentlyLoadedLevel = "NONE";
	// �÷��̾� ���� ���� ������
	UPROPERTY()
	FSaveActorData PlayerData;

public:

	/// ������ ���� ������ ��ü ����
	void SetSaveActorData(TMap<FGuid, FSaveActorData> Data);
	/// ����� ���� ������ ��ü ��ȯ
	TMap<FGuid, FSaveActorData> GetSaveActorData();

	/// ���� ���� �̸� ����
	void SetCurrentLevel(const FName Level);
	/// ����� ���� ���� �̸� ��ȯ
	FName GetCurrentLevel();

	/// �÷��̾� ������ ����
	void SetPlayerData(FSaveActorData Data);
	/// ����� �÷��̾� ������ ��ȯ
	FSaveActorData GetPlayerData();
};
