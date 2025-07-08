// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SaveActorInterface.h"

// ���� ����(pure virtual)�� �ƴ� ISaveActorInterface �Լ����� �⺻ ����� �����մϴ�.

// ������ ����� ���� �ĺ��ڸ� ��ȯ
// �⺻ ������ ����ȿ��(Invalid)�� GUID�� �����־�, 
// ���� ������ ���ϴ� Ŭ�������� �ݵ�� �������̵��ϵ��� �����մϴ�.
FGuid ISaveActorInterface::GetActorSaveID_Implementation()
{
	FGuid Ret;        // �⺻ ���� �ÿ��� ��ȿ�� ���� �ƴ�
	Ret.Invalidate(); // ��������� ��ȿȭ
	return Ret;
}

// �ܺο��� ���޵� GUID�� ���Ϳ� ����
// �⺻ ������ �ƹ� ���۵� ���� �����Ƿ�, �ʿ� �� �Ļ� Ŭ�������� �������̵��ϼ���.
void ISaveActorInterface::SetActorGUID_Implementation(const FGuid& NewGuid)
{
	// ���� ���� (�Ļ� Ŭ�������� ó��)
}

// ���� ��ü�� ���� �����͸� ��ȯ
// �⺻ ������ ���� FSaveActorData ����ü�� ��ȯ�մϴ�.
FSaveActorData ISaveActorInterface::GetSaveData_Implementation()
{
	return FSaveActorData();
}

// ���Ͱ� ������ ������Ʈ�� ���� �����͸� ��ȯ
// �⺻ ������ ���� FSaveComponentData ����ü�� ��ȯ�մϴ�.
FSaveComponentData ISaveActorInterface::GetComponentSaveData_Implementation()
{
	return FSaveComponentData();
}

// �ܺο��� ���޵� ������Ʈ ���� �����͸� ����
// �⺻ ������ �ƹ� ���۵� ���� �����Ƿ�, �ʿ� �� �Ļ� Ŭ�������� �������̵��ϼ���.
void ISaveActorInterface::SetComponentSaveData_Implementation(FSaveComponentData Data)
{
	// ���� ���� (�Ļ� Ŭ�������� ó��)
}