// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackCameraShake.h"

UBossAttackCameraShake::UBossAttackCameraShake()
{
	OscillationDuration = 1.f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;
	LocOscillation.X.Amplitude = 13.f;
	LocOscillation.X.Frequency = 100.f;
	LocOscillation.Y.Amplitude = 13.f;
	LocOscillation.Y.Frequency = 100.f;
	LocOscillation.Z.Amplitude = 13.f;
	LocOscillation.Z.Frequency = 100.f;
}
