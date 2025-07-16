// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crazy6/DebugMacros.h"

template<typename ObjectType>
class CRAZY6_API CSingleton
{
public:
	// Init Instance
	static ObjectType* Inst()
	{
		if (nullptr == mInstance)
		{
			mInstance = new ObjectType();
		}

		return mInstance;
	}

	// Get Instance, If you call to Init Func
	static ObjectType* GetInst()
	{
		if (nullptr == mInstance)
		{
			LOG_FATAL(TEXT("Not Access"));
		}

		return mInstance;
	}

	// Free Instance,
	static void Destroy()
	{
		if (nullptr == mInstance)
		{
			LOG_FATAL(TEXT("Already Free Inst"))
		}

		delete mInstance;
		mInstance = nullptr;
	}

private:
	static inline ObjectType* mInstance = nullptr;

protected:
	CSingleton()
	{

	}

	virtual ~CSingleton()
	{
		if (nullptr != mInstance)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	// Delete Func
	CSingleton(const CSingleton& _Other) = delete;
	CSingleton(CSingleton&& _Other) noexcept = delete;
	CSingleton& operator=(const CSingleton& _Other) = delete;
	CSingleton& operator=(CSingleton&& _Other) noexcept = delete;

private:

};

