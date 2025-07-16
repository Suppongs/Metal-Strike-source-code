#pragma once
#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

// =======================================
// Draw shpere for debug 
// =======================================

/** Draw Debug Shpere to World Space */
#define DRAW_SPHERE(Location)                             if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);
#define DRAW_SPHERE_COLOR(Location, Color)                if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);
#define DRAW_SPHERE_SingleFrame(Location)                 if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_BOX(Location, Extent)                        if (GetWorld()) DrawDebugBox(GetWorld(), Location, Extent, FColor::Red, true);
#define DRAW_BOX_SingleFrame(Location, Extent)            if (GetWorld()) DrawDebugBox(GetWorld(), Location, Extent, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_LINE(StartLocation, EndLocation)             if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location)                              if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_POINT_SingleFrame(Location)                  if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld())	\
	{															\
		DRAW_LINE(StartLocation, EndLocation);					\
		DRAW_POINT(Location);									\
	}
#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) if (GetWorld())	\
	{																		\
		DRAW_LINE_SingleFrame(StartLocation, EndLocation);	                \
		DRAW_POINT_SingleFrame(Location) ;				                    \
	}

// =======================================
// Draw shpere for debug 
// =======================================


DECLARE_LOG_CATEGORY_EXTERN(CustomLog, Log, All)

#define MSG_LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define LOG_S(Verbosity)				            UE_LOG(CustomLog, Verbosity, TEXT("%s"), *MSG_LOG_CALLINFO)
#define LOG_MSG(Format, ...)		                UE_LOG(CustomLog, Log, TEXT("%s %s"), *MSG_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_WARNING(Format, ...)		            UE_LOG(CustomLog, Warning, TEXT("%s %s"), *MSG_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_ERROR(Format, ...)		                UE_LOG(CustomLog, Error, TEXT("%s %s"), *MSG_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_FATAL(Format, ...)		                UE_LOG(CustomLog, Fatal, TEXT("%s %s"), *MSG_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_VERBOSE(Format, ...)		            UE_LOG(CustomLog, Verbose, TEXT("%s %s"), *MSG_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_VERYVERBOSE(Format, ...)		        UE_LOG(CustomLog, VeryVerbose, TEXT("%s %s"), *MSG_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_ALL(Format, ...)		                UE_LOG(CustomLog, All, TEXT("%s %s"), *MSG_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))


// NoLogging = 0,
// Fatal,							// 파일 및 에디터 로그에 빨간색으로 출력, 프로그램 종료
// Error,							// 파일 및 에디터 로그에 빨간색으로 출력
// Warning,							// 파일 및 에디터 로그에 노란색으로 출력
// Display,							// 파일 및 콘솔에 출력
// Log,								// 파일 및 에디터 로그에 출력
// Verbose,							// 로그 파일에 출력
// VeryVerbose,						// 로그 파일에 상세 내용을 포함하여 출력
// All = VeryVerbose,
// NumVerbosity,
// VerbosityMask = 0xf,
// SetColor = 0x40,
// BreakOnLog = 0x80,


// =============

