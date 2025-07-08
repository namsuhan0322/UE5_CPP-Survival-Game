// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TFSaveGame.h"
#include "Interface/SaveActorInterface.h"

/// ������ ���� �����͵��� ����
void UTFSaveGame::SetSaveActorData(TMap<FGuid, FSaveActorData> Data)
{
	SaveableActorData = Data;
}

/// ����� ���� �����͵��� ��ȯ
TMap<FGuid, FSaveActorData> UTFSaveGame::GetSaveActorData()
{
	return SaveableActorData;
}

/// ���� �ε�� ���� �̸��� ����
void UTFSaveGame::SetCurrentLevel(const FName Level)
{
	CurrentlyLoadedLevel = Level;
}

/// ���� ����� ���� �̸��� ��ȯ
FName UTFSaveGame::GetCurrentLevel()
{
	return CurrentlyLoadedLevel;
}

/// �÷��̾� ���� �����͸� ����
void UTFSaveGame::SetPlayerData(FSaveActorData Data)
{
	PlayerData = Data;
}

/// ����� �÷��̾� �����͸� ��ȯ
FSaveActorData UTFSaveGame::GetPlayerData()
{
	return PlayerData;
}