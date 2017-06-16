// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorFall.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, FloorFall, "FloorFall" );


//General Log
DEFINE_LOG_CATEGORY(FloorFallLog);

//Logging during game startup
DEFINE_LOG_CATEGORY(FloorFallInit);

//Logging for your AI system
DEFINE_LOG_CATEGORY(FloorFallAI);

//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(FloorFallCriticalErrors);


//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(FloorFallDebug);