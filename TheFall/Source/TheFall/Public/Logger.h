// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <Windows.h>
#include <fstream>
#include <chrono>
#include <iostream>

// �α� ���� �̸� ���λ� ����
#define LOGPREFACE "\\TFLog_"

// �α� �޽��� �ɰ��� ������ �����ϴ� ������
enum ERRORLEVEL
{
	EL_DEBUG = 0,       // ����� �޽���
	EL_WARNING = 1,     // ��� �޽���
	EL_ERROR = 2,       // ���� �޽���
	EL_CRITICAL = 3     // ġ������ ���� �޽���
};

// �α׸� ���Ϸ� ����ϴ� �̱��� Logger Ŭ����
class Logger
{
private:
	// �̱��� �ν��Ͻ� ������
	static Logger* pInstance;

	// �α� ���� ��θ� �����ϴ� �Լ�
	std::string Path()
	{
		std::string Ret;
		char ExecPath[FILENAME_MAX];

		// ���� ���� ��θ� ������
		GetCurrentDirectoryA(sizeof(ExecPath), ExecPath);
		Ret = ExecPath;
		Ret += LOGPREFACE; // ��: "\TFLog_"
		Ret += TodayDateAsString(); // ��: "2025-07-08"
		Ret += ".txt"; // Ȯ���� ���̱�

		return Ret; // ��: "C:\������Ʈ���\TFLog_2025-07-08.txt"
	}

#pragma warning(disable:4996) // localtime ���� ��� ����

	// ���� ��¥�� ���ڿ�(yyyy-mm-dd)�� ��ȯ
	std::string TodayDateAsString()
	{
		std::string ret;
		time_t Now;
		time(&Now);
		struct tm* pTime;
		pTime = std::localtime(&Now);

		ret += int2string(pTime->tm_year + 1900); // ��
		ret += "-";
		ret += int2string(pTime->tm_mon + 1);     // ��
		ret += "-";
		ret += int2string(pTime->tm_mday);        // ��
		return ret;
	}

	// ���� �ð�(��:��:��)�� ���ڿ��� ��ȯ
	std::string GetCurrentTimeStamp()
	{
		std::string ret;
		time_t Now;
		time(&Now);
		struct tm* pTime;
		pTime = std::localtime(&Now);

		ret += int2string(pTime->tm_hour); // ��
		ret += ":";
		ret += int2string(pTime->tm_min);  // ��
		ret += ":";
		ret += int2string(pTime->tm_sec);  // ��
		return ret;
	}

#pragma warning(default:4996) // ��� �ٽ� ��

	// ������ ���ڿ��� ��ȯ�ϸ�, �ʿ�� 0�� �ٿ� �ڸ��� ����
	std::string int2string(const int& i, bool padded = true)
	{
		std::ostringstream format_message;
		format_message << i;
		std::string ret = format_message.str();
		if (padded && ret.length() == 1)
		{
			ret = "0" + ret; // �� �ڸ� ���ڸ� �տ� 0 �߰�
		}
		return ret;
	}

	// ���� ������ ���� ���ڿ��� ��ȯ
	std::string ErrorLevelAsString(ERRORLEVEL level)
	{
		switch (level)
		{
		case ERRORLEVEL::EL_DEBUG:
			return "[DEBUGGING]";
		case ERRORLEVEL::EL_WARNING:
			return "[WARNING]";
		case ERRORLEVEL::EL_ERROR:
			return "[*ERROR*]";
		case ERRORLEVEL::EL_CRITICAL:
			return "[!!! Critical Error !!!]";
		default:
			return "UNKNOWN ERROR LEVLE"; // ��Ÿ: LEVEL�� ����
		}
	}

public:
	// Logger �̱��� �ν��Ͻ��� ��ȯ
	static Logger* GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new Logger();
		}
		return pInstance;
	}

	// �α� �޽����� �α� ���Ͽ� ���
	void AddMessage(std::string Msg, ERRORLEVEL level)
	{
		std::string LogLine;
		LogLine += TodayDateAsString(); // ��¥
		LogLine += " ";
		LogLine += GetCurrentTimeStamp(); // �ð�
		LogLine += "; ";
		LogLine += ErrorLevelAsString(level); // �α� ���� ���ڿ�
		LogLine += Msg; // �޽��� ����
		LogLine += "\n";

		std::ofstream fout;
		std::string path = Path(); // �α� ���� ���

		// ������ append ���� ���� �α� �ۼ�
		fout.open(path, std::ios_base::app);
		fout << LogLine;
		fout.close();
	}
};
