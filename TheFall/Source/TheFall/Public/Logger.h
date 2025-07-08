// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <Windows.h>
#include <fstream>
#include <chrono>
#include <iostream>

// 로그 파일 이름 접두사 정의
#define LOGPREFACE "\\TFLog_"

// 로그 메시지 심각도 수준을 정의하는 열거형
enum ERRORLEVEL
{
	EL_DEBUG = 0,       // 디버깅 메시지
	EL_WARNING = 1,     // 경고 메시지
	EL_ERROR = 2,       // 오류 메시지
	EL_CRITICAL = 3     // 치명적인 오류 메시지
};

// 로그를 파일로 기록하는 싱글톤 Logger 클래스
class Logger
{
private:
	// 싱글톤 인스턴스 포인터
	static Logger* pInstance;

	// 로그 파일 경로를 생성하는 함수
	std::string Path()
	{
		std::string Ret;
		char ExecPath[FILENAME_MAX];

		// 현재 실행 경로를 가져옴
		GetCurrentDirectoryA(sizeof(ExecPath), ExecPath);
		Ret = ExecPath;
		Ret += LOGPREFACE; // 예: "\TFLog_"
		Ret += TodayDateAsString(); // 예: "2025-07-08"
		Ret += ".txt"; // 확장자 붙이기

		return Ret; // 예: "C:\프로젝트경로\TFLog_2025-07-08.txt"
	}

#pragma warning(disable:4996) // localtime 관련 경고 무시

	// 오늘 날짜를 문자열(yyyy-mm-dd)로 반환
	std::string TodayDateAsString()
	{
		std::string ret;
		time_t Now;
		time(&Now);
		struct tm* pTime;
		pTime = std::localtime(&Now);

		ret += int2string(pTime->tm_year + 1900); // 년
		ret += "-";
		ret += int2string(pTime->tm_mon + 1);     // 월
		ret += "-";
		ret += int2string(pTime->tm_mday);        // 일
		return ret;
	}

	// 현재 시간(시:분:초)을 문자열로 반환
	std::string GetCurrentTimeStamp()
	{
		std::string ret;
		time_t Now;
		time(&Now);
		struct tm* pTime;
		pTime = std::localtime(&Now);

		ret += int2string(pTime->tm_hour); // 시
		ret += ":";
		ret += int2string(pTime->tm_min);  // 분
		ret += ":";
		ret += int2string(pTime->tm_sec);  // 초
		return ret;
	}

#pragma warning(default:4996) // 경고 다시 켬

	// 정수를 문자열로 변환하며, 필요시 0을 붙여 자릿수 보정
	std::string int2string(const int& i, bool padded = true)
	{
		std::ostringstream format_message;
		format_message << i;
		std::string ret = format_message.str();
		if (padded && ret.length() == 1)
		{
			ret = "0" + ret; // 한 자리 숫자면 앞에 0 추가
		}
		return ret;
	}

	// 에러 레벨에 따라 문자열로 변환
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
			return "UNKNOWN ERROR LEVLE"; // 오타: LEVEL이 맞음
		}
	}

public:
	// Logger 싱글톤 인스턴스를 반환
	static Logger* GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new Logger();
		}
		return pInstance;
	}

	// 로그 메시지를 로그 파일에 기록
	void AddMessage(std::string Msg, ERRORLEVEL level)
	{
		std::string LogLine;
		LogLine += TodayDateAsString(); // 날짜
		LogLine += " ";
		LogLine += GetCurrentTimeStamp(); // 시간
		LogLine += "; ";
		LogLine += ErrorLevelAsString(level); // 로그 레벨 문자열
		LogLine += Msg; // 메시지 본문
		LogLine += "\n";

		std::ofstream fout;
		std::string path = Path(); // 로그 파일 경로

		// 파일을 append 모드로 열고 로그 작성
		fout.open(path, std::ios_base::app);
		fout << LogLine;
		fout.close();
	}
};
