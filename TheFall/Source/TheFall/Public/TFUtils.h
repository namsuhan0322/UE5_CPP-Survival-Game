// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/// ���ڿ��� Ư�� ���� �������� ������ �迭�� ��ȯ�ϴ� ��ƿ��Ƽ �Լ� ����
/// @param Source ���� ���ڿ�
/// @param split ���� ���� ���� (�⺻��: '|')
/// @return ���ҵ� ���ڿ� �������� �迭
TArray<FString> ChopString(FString Source, char split = '|');