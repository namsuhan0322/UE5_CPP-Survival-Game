// Fill out your copyright notice in the Description page of Project Settings.


#include "TFUtils.h"

/// ���ڿ��� Ư�� ���� �������� ������ �迭�� ��ȯ�ϴ� ��ƿ��Ƽ �Լ�
/// @param Source ���� ���� ���ڿ�
/// @param split ������ �Ǵ� ���� (��: ',', ':', '|')
/// @return ���ҵ� ���ڿ� �������� �迭
TArray<FString> ChopString(FString Source, char split)
{
	TArray<FString> Ret;    // ����� ������ �迭
	FString LSide, RSide;   // ����/������ ���ڿ� �ӽ� �����

	// split ���ڸ� �������� ���ڿ��� ��� ����
	while (Source.Split(&split, &LSide, &RSide))
	{
		Ret.Add(LSide);    // ���� ���ڿ��� ��� �迭�� �߰�
		Source = RSide;    // ������ ������ ���ڿ��� �ݺ� ����
		LSide.Empty();     // ���� ������ ���� �ʱ�ȭ
	}

	// ������ ���� ���ڿ��� �ִٸ� �߰�
	if (!RSide.IsEmpty())
	{
		Ret.Add(RSide);
	}

	return Ret;
}