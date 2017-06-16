// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//General Log
DECLARE_LOG_CATEGORY_EXTERN(FloorFallLog, Log, All);

//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(FloorFallInit, Log, All);

//Logging for your AI system
DECLARE_LOG_CATEGORY_EXTERN(FloorFallAI, Log, All);

//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(FloorFallCriticalErrors, Log, All);

//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(FloorFallDebug, Log, All);